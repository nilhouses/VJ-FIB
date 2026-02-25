#include <iostream>
#include <cmath>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "PlayScene.h"
#include "Game.h"

#define SCREEN_X 32
#define SCREEN_Y 16
#define INIT_PLAYER_X_TILES 5
#define INIT_PLAYER_Y_TILES 6
const int HUD_HEIGHT = 96;

// Constructor: guardamos el nivel actual
PlayScene::PlayScene(int levelNumber)
{
    map = nullptr;
    player = nullptr;
    level = levelNumber;
}

PlayScene::~PlayScene()
{
    texProgram.free();
    if (map != NULL)
        delete map;
    if (player != NULL)
        delete player;
}

void PlayScene::init()
{
    cameraTarget.x = SCREEN_WIDTH / 2;
    cameraTarget.y = (SCREEN_HEIGHT - HUD_HEIGHT) / 2;
    cameraPos = glm::vec2(0.f, 0.f);

    initShaders();
    // Asumimos que los niveles del 1 al 9 tienen un 0 delante
    string levelPath = "levels/level0" + std::to_string(level) + ".txt";
    string entityPath = "entity/level0" + std::to_string(level) + ".txt";
    // Carga el mapa a través de un fichero de texto
    map = TileMap::createTileMap(levelPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    // Crea el jugador y lo sitúa en la posición inicial
    player = new Player();
    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
    player->setTileMap(map);
    // La matriz de proyección es ortogonal con el tamaño de la pantalla
    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
    currentTime = 0.0f;

    entities = vector<Entity*>();
    collectedKeys = 0;
    levelCompleted = false;
    
    // Fijar la cámara en el jugador
    cameraPos.x = player->getPosition().x * map->getTileSize() - cameraTarget.x;
    cameraPos.y = player->getPosition().y * map->getTileSize() - cameraTarget.y;
}

// En esta escena solo se mueve el jugador
void PlayScene::update(int deltaTime)
{
    currentTime += deltaTime;
    // Actualizar al jugador
    player->update(deltaTime);
    // Actualizar al resto de entidades
    for (Entity* entity : entities)
    {
        if (entity->isActive())
            entity->update(deltaTime);
	}

	// Actualizar la posición de la cámara para que siga al jugador, pero sin salirnos del mapa
    cameraPos.x = player->getPosition().x * map->getTileSize() - cameraTarget.x;
    cameraPos.y = player->getPosition().y * map->getTileSize() - cameraTarget.y;
    float mapWidth = map->getMapSize().x * map->getTileSize();
    float mapHeight = map->getMapSize().y * map->getTileSize();
    cameraPos.x = glm::clamp(cameraPos.x, 0.f, mapWidth - SCREEN_WIDTH);
    cameraPos.y = glm::clamp(cameraPos.y, 0.f, mapHeight - (SCREEN_HEIGHT - HUD_HEIGHT));
    cameraPos = glm::floor(cameraPos);
}

void PlayScene::render()
{
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
    /*
    glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-cameraPos.x,
            -cameraPos.y - HUD_HEIGHT,
            0.f)
    );
    */
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    /*
        Orden de renderizado:
        1. Fondo
        2. Mapa
        3. Entidades
        4. Jugador
		5. Interfaz (vida, llaves, etc.)
    */
    map->render();
    for (Entity* entity : entities)
    {
        if (entity->isActive())
            entity->render();
	}
    player->render();
}

void PlayScene::initShaders()
{
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}