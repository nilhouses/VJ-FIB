
#include "PlayScene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

// Tamaño de cámara
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
#define HUD_HEIGHT 0

// Offset del mapa
#define SCREEN_X 0
#define SCREEN_Y 32

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

void PlayScene::createEntity(const string& type, int tx, int ty, Camera* c)
{
    Entity* entity = nullptr;

    if (type == "PLAYER")
    {
        player = new Player();
        player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, c);
        player->setTileMap(map);
		entity = player;
    }
    else if (type == "KEY")
    {
        Key* key = new Key();
        key->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, c);
        entity = key;
    }
    else if (type == "WEIGHT")
    {
        Weight* weight = new Weight();
        weight->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, c);
        weight->setTileMap(map);
        entity = weight;
    }

    if (entity != nullptr)
    {
		entity->setPosition(glm::vec2(float(tx * map->getTileSize()), float(ty * map->getTileSize())));
        entities.push_back(entity);
    }
}

void PlayScene::loadEntities(const string& entityPath, Camera* c)
{
    ifstream fin(entityPath);

    string type;
    int count;

    while (fin >> type)
    {
        fin >> count;

        for (int i = 0; i < count; ++i)
        {
            int tileX, tileY;
            fin >> tileX >> tileY;

            createEntity(type, tileX, tileY, c);
        }
    }
}

void PlayScene::init()
{
    initShaders();
	// Atributos iniciales de la escena
    entities = vector<Entity*>();
    collectedKeys = 0;
    levelCompleted = false;

    // Asumimos que los niveles del 1 al 9 tienen un 0 delante
    string levelPath = "levels/level0" + std::to_string(level) + "/map.txt";
    string entityPath = "levels/level0" + std::to_string(level) + "/entities.txt";

    // Carga el mapa a través de un fichero de texto
    map = TileMap::createTileMap(levelPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

    // Carga las entidades
    camera = new Camera(CAMERA_WIDTH, CAMERA_HEIGHT, HUD_HEIGHT);
	loadEntities(entityPath, camera);

    // La matriz de proyección es ortogonal
    projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);
    currentTime = 0.0f;
}

// Funcion Auxiliar para checkCollisions()
bool overlap(const glm::vec4& a, const glm::vec4& b)
{
    return !(a.x + a.z < b.x ||
        b.x + b.z < a.x ||
        a.y + a.w < b.y ||
        b.y + b.w < a.y);
}

void PlayScene::checkCollisions()
{
    auto playerBox = player->getBoundingBox();

    for (Entity* e : entities)
    {
        if (!e->isActive()) continue;

        if ((e->getType() != player->getType()) &&  overlap(playerBox, e->getBoundingBox()))
        {
            handlePlayerCollision(e);
        }
    }
}

void PlayScene::handlePlayerCollision(Entity* e)
{
    switch (e->getType())
    {
        case Type::KEY:
            collectedKeys++;
            e->deactivate();
			cout << "collectedKeys: " << collectedKeys << endl;
            break;

        case Type::WEIGHT:

            Weight* w = static_cast<Weight*>(e);
            
            float playerBottom = player->getPosition().y + (player->getBoundingBox().w);
            float weightTop = w->getPosition().y;
            float weightBottom = w->getPosition().y + (w->getBoundingBox().w);

            // Colisión vertical
            if (weightTop <= playerBottom && weightBottom > playerBottom) {
                player->incrUp(playerBottom - weightTop);
            }
            else {
                // Colisión horizontal En función del player se empuja para un lado o otro
                float xPlayer = player->getPosition().x;
                float xWeight = e->getPosition().x;
                // Castear a clase Weight, entity no tiene la función
                Weight* w = static_cast<Weight*>(e);
                if (xPlayer < xWeight) {
                    if (!w->incrRight(player->getSpeed())) player->incrLeft();
                }
                else {
                    if (!w->incrLeft(player->getSpeed())) player->incrRight();
                }
            }
            break;
    }
}

// En esta escena solo se mueve el jugador
void PlayScene::update(int deltaTime)
{
    currentTime += deltaTime;

    // Actualizar entidades
    for (Entity* entity : entities)
    {
        if (entity->isActive())
            entity->update(deltaTime);
	}

    checkCollisions();

	camera->update(player->getPosition(), map->getMapSize() * map->getTileSize());
}


void PlayScene::render()
{
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::translate(
        glm::mat4(1.0f),
		glm::vec3(camera->getOffset(), 0.0f)
    );
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    /*
        Orden de renderizado:
        1. Fondo
        2. Mapa
        3. Entidades
		4. Jugador (añadir siempre al final del fichero de entidades para que se renderice después que el resto)
		5. Interfaz (vida, llaves, etc.)
    */
    map->render();
    for (Entity* entity : entities)
    {
        if (entity->isActive())
            entity->render();
	}
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