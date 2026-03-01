#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

// Offset del mapa
#define SCREEN_X 0
#define SCREEN_Y 0

// Cooldowns entre interacciones
float roomChangeCooldown = 0.f;


// ---------------------- CONSTRUCTORS ----------------------

Level::Level(int levelNumber)
{
    level = levelNumber;
}

Level::~Level()
{
    texProgram.free();
    for (Room* room : rooms) {
        if (room != nullptr)
            delete room;
	}
}

// ---------------------- INIT ----------------------

void Level::initShaders()
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


void Level::loadGlobalInfo(const string& globalInfoPath)
{
    ifstream fin(globalInfoPath);
	string property;

    while (fin >> property)
    {
        if (property == "ALL_KEYS")
        {
            fin >> allKeys;
        }
        else if (property == "NUM_ROOMS")
        {
            fin >> numRooms;
		}
        // Aquí se pueden añadir más propiedades globales del nivel
	}
}


void Level::loadRooms()
{
    for (int i = 0; i <= numRooms; ++i)
    {
        Room* room = new Room(texProgram, projection);
        string levelPath = "levels/level0" + std::to_string(level) + "/room0" + std::to_string(i) + "/map.txt";
        string entityPath = "levels/level0" + std::to_string(level) + "/room0" + std::to_string(i) + "/entities.txt";
        string assetPath = "levels/level0" + std::to_string(level) + "/room0" + std::to_string(i) + "/assets.txt";
        room->init(glm::vec2(SCREEN_X, SCREEN_Y), levelPath, entityPath, assetPath);
        rooms.push_back(room);
    }
}


void Level::init()
{
    initShaders();

	// Atributos globales del nivel
	collectedKeys = 0;
	string globalInfoPath = "levels/level0" + std::to_string(level) + "/globalInfo.txt";
    loadGlobalInfo(globalInfoPath);
	levelCompleted = false;
	rooms = vector<Room*>();
	currentRoom = 0;

    loadRooms();

    currentTime = 0.0f;
}

// ---------------------- UPDATE + COLLISIONS ----------------------

bool Level::overlap(const glm::vec4& a, const glm::vec4& b, const glm::vec2& offset)
{
    return !(a.x + a.z < b.x + offset.x ||
        b.x + b.z < a.x + offset.x ||
        a.y + a.w < b.y + offset.y ||
        b.y + b.w < a.y + offset.y);
}


void Level::handlePlayerCollision(Player* player, Entity* e)
{
    switch (e->getType())
    {
    case Type::KEY:
        collectedKeys++;
        e->deactivate();
        cout << "collectedKeys: " << collectedKeys << endl;
        break;

    case Type::WEIGHT:
    {
        // Castear a clase Weight, entity no tiene la función
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
            if (xPlayer < xWeight) {
                if (!w->incrRight(player->getSpeed())) player->incrLeft();
            }
            else {
                if (!w->incrLeft(player->getSpeed())) player->incrRight();
            }
        }
        break;
    }
    case Type::DOOR:
		int centerX = player->getPosition().x + player->getBoundingBox().z / 2;
        if (Game::instance().getKey(GLFW_KEY_UP) && roomChangeCooldown == 0 && Utils::isCentered(centerX)) {

			// Animación de entrar a la puerta
			currentRoom = static_cast<Door*>(e)->getRoomTo();
			// Animación de salir de la puerta

			roomChangeCooldown = 1000.f;     // Cooldown de dos segundos
        }
        break;
    }
}


void Level::checkCollisions()
{
	// Comprobar colisiones entre el jugador y las entidades del nivel actual
	Player* player = rooms[currentRoom]->getPlayer();
    vector<Entity*>& entities = rooms[currentRoom]->getEntities();

    auto playerBox = player->getBoundingBox();

    for (Entity* e : entities)
    {
        if (!e->isActive()) continue;

        glm::vec2 offset(0.f, 0.f);
        if (e->getType() == Type::KEY) {
            offset = glm::vec2(8.f, 8.f);
        }

        if ((e->getType() != player->getType()) && overlap(playerBox, e->getBoundingBox(), offset))
        {
            handlePlayerCollision(player, e);
        }
    }
}


void Level::update(int deltaTime)
{    
    currentTime += deltaTime;
	roomChangeCooldown = std::max(0.f, roomChangeCooldown - deltaTime);
    
    // Actualizo la habitación actual, que a su vez actualiza el jugador y el resto de entidades.
	rooms[currentRoom]->update(deltaTime);
    // Analizamos si el jugador de la habitación actual ha interactuado con alguna entidad
    checkCollisions();
}


// ---------------------- RENDER ----------------------

void Level::render()
{
    // Se renderiza solo la habitación actual en su estado actual
	rooms[currentRoom]->render();
}
