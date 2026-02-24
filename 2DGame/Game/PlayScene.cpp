#include <iostream>
#include <cmath>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "PlayScene.h"
#include "Game.h"

#define SCREEN_X 32
#define SCREEN_Y 16
#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

// Constructor: guardamos el nivel actual
PlayScene::PlayScene(int levelNumber)
{
    map = NULL;
    player = NULL;
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
    initShaders();
    // Asumimos que los niveles del 1 al 9 tienen un 0 delante
    string levelPath = "levels/level0" + std::to_string(level) + ".txt";
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
}

void PlayScene::update(int deltaTime)
{
    currentTime += deltaTime;
    player->update(deltaTime);
}

void PlayScene::render()
{
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    map->render();
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