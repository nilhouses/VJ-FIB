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

// Tamaño de cámara
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
#define HUD_HEIGHT 0


enum LevelState { NORMAL, ENTERING_DOOR, EXITING_DOOR, DYING};

// Cooldowns entre interacciones
float transitionTimer = 0.f;


// ---------------------- CONSTRUCTORS ----------------------

Level::Level(int levelNumber) : Scene(SceneType::LEVEL)
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

Entity* Level::createEntity(const string& type, int tx, int ty, int indexRoom)
{
    Entity* entity = nullptr;
	TileMap* map = rooms[indexRoom]->getMap();

    if (type == "PLAYER")
    {
        player = new Player();
        player->init(glm::vec2(SCREEN_X,SCREEN_Y), texProgram, camera);
        player->setTileMap(map);
        entity = player;
    }
    else if (type == "KEY")
    {
        Key* key = new Key();
        key->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        entity = key;
    }
    else if (type == "WEIGHT")
    {
        Weight* weight = new Weight();
        weight->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera, glm::vec2(float(tx * map->getTileSize()), float(ty * map->getTileSize())));
        weight->setTileMap(map);
        entity = weight;
    }
    else if (type == "DOOR")
    {
        Door* door = new Door();
        door->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        if (indexRoom != 0)
            door->setToVisited();
        entity = door;
    }
    else if (type == "DUMMY")
    {
        Dummy* dummy = new Dummy();
        dummy->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        dummy->setTileMap(map);
        entity = dummy;
        // Añado al vector de enemigos
		rooms[indexRoom]->addEnemy(dummy);
    }
    // ...

    // Común para todas las entidades
    if (entity != nullptr)
    {
        entity->setPosition(glm::vec2(float(tx * map->getTileSize()), float(ty * map->getTileSize())));
		entity->setRoom(indexRoom);
		// Añado la entidad a la habitación correspondiente
        if (type != "PLAYER") rooms[indexRoom]->addEntity(entity);
    }

	return entity;
}


void Level::loadEntities()
{
	string entityPath = "levels/level0" + std::to_string(level) + "/entities.txt";
    ifstream fin(entityPath);

    string type;
    int count;

    while (fin >> type)
    {
        fin >> count;

        for (int i = 0; i < count; ++i)
        {
            int indexRoom, tileX, tileY;
            fin >> indexRoom >> tileX >> tileY;

            Entity* e1 = createEntity(type, tileX, tileY, indexRoom);
            
            // Si la entidad es una puerta leo su puerta conectada para enlazarlas
            if (type == "DOOR") {
                fin >> indexRoom >> tileX >> tileY;
                Entity* e2 = createEntity(type, tileX, tileY, indexRoom);

                Door* d1 = static_cast<Door*>(e1);
                Door* d2 = static_cast<Door*>(e2);

				d1->setDoorTo(d2);
				d2->setDoorTo(d1);
            }
        }
    }
}


void Level::createAsset(const string& spriteDir, glm::vec2& pos, glm::vec2& size, int indexRoom)
{
    Asset* asset = new Asset();
    asset->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, spriteDir, size, camera);
	int tileSize = rooms[indexRoom]->getMap()->getTileSize();
    asset->setPosition(glm::vec2(float(pos.x * tileSize), float(pos.y * tileSize)));
	// Añado el asset a la habitación correspondiente
	rooms[indexRoom]->addAsset(asset);
}


void Level::loadAssets()
{
	string assetPath = "levels/level0" + std::to_string(level) + "/assets.txt";
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
            int indexRoom, tileX, tileY;
            fin >> indexRoom >> tileX >> tileY;

            createAsset(path, glm::vec2(tileX, tileY), glm::vec2(sizeX, sizeY), indexRoom);
        }
    }
}


void Level::loadMaps(vector<TileMap*>& maps, int totalMaps)
{
    for (int i = 0; i < totalMaps; ++i)
    {
        string mapPath = "levels/level0" + std::to_string(level) + "/maps/map" + std::to_string(i) + ".txt";
        maps[i] = TileMap::createTileMap(mapPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
}


void Level::createRooms()
{
    string indexPath = "levels/level0" + std::to_string(level) + "/indexMapToRoom.txt";
	ifstream fin(indexPath);

	int totalRooms, totalMaps;
	fin >> totalRooms >> totalMaps; // Número de Rooms y Maps distintos

	vector<TileMap*> maps(totalMaps);
	loadMaps(maps, totalMaps);  // Se cargan los mapas y se guardan sus referencias en el vector maps para asignarlos a cada habitación

	int roomIndex, mapIndex;
    for (int i = 0; i < totalRooms; ++i)
    {
        fin >> roomIndex >> mapIndex;
		Room* room = new Room(texProgram);
        room->init();
		room->setMap(maps[mapIndex]); // Se asigna el mapa correspondiente a la habitación
		rooms.push_back(room);
	}

	// Se cargan las entidades y los assets después de crear las habitaciones para poder asignar cada entidad a su habitación correspondiente
	loadAssets();
    loadEntities();
}


void Level::init()
{
    initShaders();

	// Atributos globales del nivel
	collectedKeys = 0;
	string globalInfoPath = "levels/level0" + std::to_string(level) + "/globalInfo.txt";
	levelCompleted = false;
	rooms = vector<Room*>();
	currentRoom = 0;
    state = NORMAL;
    camera = new Camera(CAMERA_WIDTH, CAMERA_HEIGHT, HUD_HEIGHT);
    projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);

    createRooms();

    currentTime = 0.0f;
}

// ---------------------- UPDATE + COLLISIONS ----------------------

CollisionInfo Level::overlap(const glm::vec4& a, const glm::vec4& b, const glm::vec2& offset)
{
    CollisionInfo info;
    
    bool colliding = !(a.x + a.z < b.x + offset.x ||
                       b.x + b.z < a.x + offset.x ||
                       a.y + a.w < b.y + offset.y ||
                       b.y + b.w < a.y + offset.y);
    
    info.colliding = colliding;
    
    if (colliding) {
        // Calcular rango horizontal de colisión
        float leftMax = std::max(a.x + offset.x, b.x + offset.x);
        float rightMin = std::min(a.x + a.z, b.x + b.z);
        float horizontalOverlap = rightMin - leftMax;
        
        // Calcular rango vertical de colisión
        float topMax = std::max(a.y + offset.y, b.y + offset.y);
        float bottomMin = std::min(a.y + a.w, b.y + b.w);
        float verticalOverlap = bottomMin - topMax;
        
        info.rangeColision = glm::vec2(horizontalOverlap, verticalOverlap);
    } else {
        info.rangeColision = glm::vec2(0.f, 0.f);
    }
    
    return info;
}


void Level::handlePlayerCollision(Entity* e, glm::vec2& rangeCollided)
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

            // El player no trata el weight como una caja si esta está explotando
            if (w->isExploding()) break;
            
            // Colisiones
            glm::ivec2 pSize = player->getSize();
            glm::ivec2 wSize = w->getSize();

            float playerBottom = player->getPosition().y + pSize.y;
            float weightTop = w->getPosition().y;
            float weightBottom = w->getPosition().y + wSize.y;

            // Colisión vertical
            if (weightTop <= playerBottom && weightBottom > playerBottom) {
                player->incrUp(playerBottom - weightTop);
            }
            else {
                // Colisión horizontal En función del player se empuja para un lado o otro
                float xPlayer = player->getPosition().x;
                float xWeight = e->getPosition().x;
                int pushDist = 2 * rooms[currentRoom]->getMap()->getTileSize();
                if (xPlayer < xWeight) {
                    w->startPush(1, pushDist);
                    player->setPosition(glm::vec2(xWeight - wSize.x, player->getPosition().y));
                }
                else {
                    w->startPush(-1, pushDist);
                    player->setPosition(glm::vec2(xWeight + wSize.x, player->getPosition().y));
                }
            }
            break;
        }
        case Type::DOOR:
        {
            int centerX = player->getPosition().x + 16.0f;
            if (Game::instance().getKey(GLFW_KEY_UP) && state == NORMAL && rangeCollided.x > 20) {
                
                Door* door = static_cast<Door*>(e);
                
                // Configurar transición a la nueva habitación
                state = ENTERING_DOOR;
                transitionTimer = 1000.f;
				Door* targetDoor = door->getDoorTo();
                targetRoom = targetDoor->getRoom();
                targetSpawnPosition = targetDoor->getPosition();
                rooms[currentRoom]->setTransitioning(true);

                // Cambiar estado visual
                door->setToVisited();
                player->setAnimation("ENTERING_DOOR");
                player->blockInput(); // Bloquear input del jugador durante la transición
            }
            break;
        }
        case Type::DUMMY:
        {
            Dummy* d = static_cast<Dummy*>(e);
            // Si el dummy se està muriendo no puede matar a nadie
            if (!d->isDying()) {

                // Configurar transición a la nueva habitación
                state = DYING;
                transitionTimer = 1000.f;
                rooms[currentRoom]->setTransitioning(true);

                // Cambiar estado visual
                player->setAnimation("DIE");
                player->blockInput(); // Bloquear input del jugador durante la transición
            }
            break;
        }        
    }
}

void Level::handleEnemyCollision(Entity* enemy, Entity* e, glm::vec2& rangeCollided)
{
    switch (enemy->getType())
    {
        case Type::DUMMY:
        {
            Dummy* d = static_cast<Dummy*>(enemy);
            switch (e->getType())
            {
            case Type::WEIGHT:
                Weight* w = static_cast<Weight*>(e);
                if (w->isMoving()) {
					// El peso explota en la misma posición del dummy, NO al lado
                    glm::vec2 enemyPos = enemy->getPosition();
                    w->setPosition(enemyPos);
                    d->die();
                    w->stopPush();
                    w->explodeWeight();
                }
                else d->changeDirection();
                break;
            }
        }
    }
}


void Level::checkCollisions()
{
	// 1. Comprobar colisiones entre el jugador y las entidades del nivel actual
    vector<Entity*>& entities = rooms[currentRoom]->getEntities();

    auto playerBox = player->getBoundingBox();

    for (Entity* e : entities)
    {
        if (!e->isActive()) continue;

        glm::vec2 offset(0.f, 0.f);
        if (e->getType() == Type::KEY) {
            offset = glm::vec2(8.f, 8.f);
        } else if (e->getType() == Type::DUMMY) {
            offset = glm::vec2(5.f, 9.f);
		}

		CollisionInfo collision = overlap(playerBox, e->getBoundingBox(), offset);

        if ((e->getType() != player->getType()) && collision.colliding)
        {
            handlePlayerCollision(e, collision.rangeColision);
        }
    }

    // 2. Comprobar colisiones entre los enemigos y las distintas entidades del nivel actual
    vector<Entity*>& enemies = rooms[currentRoom]->getEnemies();

    for (Entity* enemy : enemies)
    {
        if (!enemy->isActive()) continue;

        // El Dummy no tiene colisiones si se está muriendo
        if (enemy->getType() == Type::DUMMY) {
            Dummy* d = static_cast<Dummy*>(enemy);
            if (d->isDying()) continue;
        }

        auto enemyBox = enemy->getBoundingBox();

        for (Entity* e : entities)
        {
            if (!e->isActive()) continue;

            // Offset según cada entity
            glm::vec2 offset(0.f, 0.f);
            CollisionInfo collision = overlap(enemyBox, e->getBoundingBox(), offset);

            if ((e->getType() != enemy->getType()) && collision.colliding)
            {
                handleEnemyCollision(enemy, e, collision.rangeColision);
            }
        }
    }
}


void Level::update(int deltaTime)
{
    currentTime += deltaTime;

    rooms[currentRoom]->update(deltaTime);
	player->update(deltaTime);

    switch(state)
    {
        case NORMAL:
            checkCollisions();
            break;

        case ENTERING_DOOR:
            transitionTimer = std::max(0.f, transitionTimer - deltaTime);

            if (transitionTimer == 0.f) {
                currentRoom = targetRoom;

				float tileSize = rooms[currentRoom]->getMap()->getTileSize();
                player->setPosition(glm::vec2(targetSpawnPosition.x, targetSpawnPosition.y));
                player->setTileMap(rooms[currentRoom]->getMap());
				player->blockInput();
                player->setAnimation("EXITING_DOOR");
                rooms[currentRoom]->setTransitioning(true);

                // Cambio de estado a EXITING_DOOR
                state = EXITING_DOOR;
                transitionTimer = 1000.f;
            }

            break;

        case EXITING_DOOR:
            transitionTimer = std::max(0.f, transitionTimer - deltaTime);

            if (transitionTimer == 0.f) {
                player->setAnimation("STAND_RIGHT");
				player->unblockInput();
                rooms[currentRoom]->setTransitioning(false);
                state = NORMAL;
            }

            break;
        case DYING:
            transitionTimer = std::max(0.f, transitionTimer - deltaTime);
            if (transitionTimer == 0.f) {
                // Animación acabada
                player->unblockInput();
                if (numLives > 0) {
                    numLives--;
					cout << "numLives: " << numLives << endl;
                    init(); // Temporal, el init vuelve a leer todos los ficheros. Necesitaremos un reset()
                }
            }
			break;
	}

	camera->update(player->getPosition(), rooms[currentRoom]->getMap()->getMapSize() * rooms[currentRoom]->getMap()->getTileSize());
}


// ---------------------- RENDER ----------------------

void Level::render()
{
    // Se renderiza solo la habitación actual en su estado actual
	rooms[currentRoom]->render(camera, projection);
	player->render();
}



bool Level::gameOver() { return (numLives == 0); }
bool Level::getLevelCompleted() { return levelCompleted; } // Se deberá poner que se haya entrado en la última puerta, con todas las llaves recogidas