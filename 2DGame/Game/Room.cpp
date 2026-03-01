#include "Room.h"
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


// ---------------------- CONSTRUCTORS ----------------------

Room::Room(ShaderProgram& shaderProgram, glm::mat4& projection)
	: texProgram(shaderProgram), projection(projection)
{
	camera = nullptr;
    player = nullptr;
    map = nullptr;
}

Room::~Room()
{
    if (map != NULL)
        delete map;
	for (Entity* entity : entities) {
        if (entity != nullptr)
            delete entity;
    }
	entities.clear();
    for (Asset* asset : assets) {
        if (asset != nullptr)
            delete asset;
	}
	assets.clear();
	if (camera != nullptr)
        delete camera;
    if (player != nullptr)
		delete player;
}

// ---------------------- INIT ----------------------

void Room::createEntity(const glm::ivec2& tileMapPos, const string& type, int tx, int ty, int roomTo, int prx, int pry)
{
    Entity* entity = nullptr;

    if (type == "PLAYER")
    {
        player = new Player();
        player->init(tileMapPos, texProgram, camera);
        player->setTileMap(map);
        entity = player;
    }
    else if (type == "KEY")
    {
        Key* key = new Key();
        key->init(tileMapPos, texProgram, camera);
        entity = key;
    }
    else if (type == "WEIGHT")
    {
        Weight* weight = new Weight();
        weight->init(tileMapPos, texProgram, camera);
        weight->setTileMap(map);
        entity = weight;
    }
    else if (type == "DOOR")
    {
        Door* door = new Door();
        door->init(tileMapPos, texProgram, camera);
		door->setRoomTo(roomTo);
        if (numRoom != 0)
            door->setToVisited();
		door->setDoorTargetPosition(glm::vec2(float(prx), float(pry)));

        entity = door;
    }
    // Aquí se pueden añadir más tipos de entidades


    // Común para todas las entidades
    if (entity != nullptr)
    {
        entity->setPosition(glm::vec2(float(tx * map->getTileSize()), float(ty * map->getTileSize())));
        entities.push_back(entity);
    }
}

void Room::loadEntities(const glm::ivec2& tileMapPos, const string& entityPath)
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

            int roomTo = -1;
            int posRelativeDoorX = -1;
            int posRelativeDoorY = -1;

			if (type == "DOOR") // Las puertas tienen info extra (número de room destino, posición relativa a la puerta a la que se teletransporta el jugador)
                fin >> roomTo >> posRelativeDoorX >> posRelativeDoorY;

            createEntity(tileMapPos, type, tileX, tileY, roomTo, posRelativeDoorX, posRelativeDoorY);
        }
    }
}


void Room::createAsset(const glm::ivec2& tileMapPos, const string& spriteDir, glm::vec2& pos, glm::vec2& size)
{
    Asset* asset = new Asset();
    asset->init(tileMapPos, texProgram, spriteDir, size, camera);
    asset->setPosition(glm::vec2(float(pos.x * map->getTileSize()), float(pos.y * map->getTileSize())));
    assets.push_back(asset);
}


void Room::loadAssets(const glm::ivec2& tileMapPos, const string& assetPath)
{
    ifstream fin(assetPath);

    string path;
    int count;

    while (fin >> path)
    {
        fin >> count;

        int sizeX, sizeY;

        fin >> sizeX >> sizeY;

        for (int i = 0; i < count; ++i)
        {
            int tileX, tileY;
            fin >> tileX >> tileY;

            createAsset(tileMapPos, path, glm::vec2(tileX, tileY), glm::vec2(sizeX, sizeY));
        }
    }
}


void Room::init(const glm::ivec2& tileMapPos, const string& levelPath, const string& entityPath, const string& assetPath, int numRoom)
{
    // Creamos la cámara para que se añada a cada elemento del mapa
    camera = new Camera(CAMERA_WIDTH, CAMERA_HEIGHT, HUD_HEIGHT);
    projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);
	this->numRoom = numRoom;
	transitioning = false;

    // Carga el mapa a través de un fichero de texto
    map = TileMap::createTileMap(levelPath, tileMapPos, texProgram);
	// Carga las entidades
    loadEntities(tileMapPos, entityPath);
	// Carga los elementos decorativos
    loadAssets(tileMapPos, assetPath);
}


// ---------------------- UPDATE -----------------------


void Room::update(int deltaTime)
{
    // Solo actualizo la animación del jugador durante la transición y se mantiene bloqueada su entrada
    if (transitioning)
		player->update(deltaTime);
    else {
        // Actualizar entidades
        for (Entity* entity : entities)
        {
            if (entity->isActive())
                entity->update(deltaTime);
        }
    }

    camera->update(player->getPosition(), map->getMapSize() * map->getTileSize());

    // Las colisiones se detectarán desde la clase global Level para actualizar el estado del juego
}


// ---------------------- RENDER ----------------------

void Room::render()
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
        2. tileMapBase
        3. Assets
        4. tileMapFront
        5. Entidades
        6. HUD
    */
    map->renderBase();

    for (Asset* asset : assets)
    {
        asset->render();
    }

    // Los assets actualizan la modelview así que devolvemos la modelview original para renderizar el tilemap front
    texProgram.setUniformMatrix4f("modelview", modelview);
    map->renderFront();

    for (Entity* entity : entities)
    {
        if (entity->isActive())
            entity->render();
    }
}