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
    else if (type == "BARREL")
    {
        Barrel* barrel = new Barrel();
        barrel->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera, glm::vec2(float(tx * map->getTileSize()), float(ty * map->getTileSize())));
        barrel->setTileMap(map);
        entity = barrel;
    }
    else if (type == "DOOR")
    {
        Door* door = new Door();
        door->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        if (indexRoom != 0)
            door->setToVisited();
        entity = door;
    }
    else if (type == "DUMMY" || type == "CLEVER" || type == "SHOOTER")
    {
        Enemy* enemy = nullptr;
        if (type == "DUMMY") enemy = new Dummy();
        else if (type == "CLEVER") enemy = new Clever();
        else if (type == "SHOOTER") {
            enemy = new Shooter();
            static_cast<Shooter*>(enemy)->setRoom(rooms[indexRoom]);
        }

        enemy->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        enemy->setTileMap(map);

        // Añado al vector de enemigos
        rooms[indexRoom]->addEnemy(enemy);
        entity = enemy;
    }


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

		if (type == "KEY") allKeys = count;

        for (int i = 0; i < count; ++i)
        {
            int indexRoom1, tileX1, tileY1;
            fin >> indexRoom1 >> tileX1 >> tileY1;

			if (type == "DOOR") {
                int indexRoom2, tileX2, tileY2;
                fin >> indexRoom2 >> tileX2 >> tileY2;

                Entity* e1 = createEntity(type, tileX1, tileY1, indexRoom1);
                Entity* e2 = createEntity(type, tileX2, tileY2, indexRoom2);

                Door* d1 = static_cast<Door*>(e1);
                Door* d2 = static_cast<Door*>(e2);

				d1->setDoorTo(d2);
				d2->setDoorTo(d1);

				bool isFinalDoor = (indexRoom1 == indexRoom2) && (tileX1 == tileX2) && (tileY1 == tileY2); // Si la puerta conecta consigo misma, es la puerta final
				d1->setIsFinalDoor(isFinalDoor);
				d2->setIsFinalDoor(isFinalDoor);
            }
            else
				createEntity(type, tileX1, tileY1, indexRoom1);
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

    for (Room* room : rooms) {
        for (Enemy* enemy : room->getEnemies()) {
            if (enemy->getEnemyType() == EnemyType::CLEVER) {
                Clever* clever = static_cast<Clever*>(enemy);
                clever->setPlayerTarget(this->player);
            }
        }
    }
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


void Level::handlePlayerCollision(Entity* e, glm::vec2& rangeCollided, glm::vec2& offset)
{
    switch (e->getType())
    {
        case Type::KEY:
            collectedKeys++;
            e->deactivate();
            cout << "collectedKeys: " << collectedKeys << "/" << allKeys << endl;
            break;

        case Type::BARREL:
        {
            Barrel* b = static_cast<Barrel*>(e);
            if (b->isExploding()) break;
            
            // Colisiones
            glm::ivec2 pSize = player->getSize();
            glm::vec2 pPos = player->getPosition();
            glm::vec2 bPos = b->getPosition();
            glm::ivec2 bSize = b->getSize();

            float playerBottom = pPos.y + pSize.y;
            float barrelTop = bPos.y;

            // Colisión vertical
            if (rangeCollided.y < rangeCollided.x && pPos.y < bPos.y)
            {
                player->incrUp(playerBottom - barrelTop - offset.y);
            }
            else
            {
                // Colisión horizontal, lógica de empuje
                int tileSize = rooms[currentRoom]->getMap()->getTileSize();
                int pushDist = 2*tileSize + (rand() % (tileSize * 2));

                if (pPos.x < bPos.x) {
                    // Estem a l'esquerra: retrocedim exactament el que hem entrat (rangeCollided.x)
                    player->setPosition(glm::vec2(pPos.x - rangeCollided.x, pPos.y));

                    if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                        if (b->tryPush(1, 1.0f, pushDist) && (player->getCurrentAnimationName() != "MOVE_RIGHT"))
                            player->setAnimation("MOVE_RIGHT");
                        else {
							if (player->getCurrentAnimationName() != "PUSH_RIGHT")
                                player->setAnimation("PUSH_RIGHT");
                        }
                    }
                    else {
                       // Hay contacto pero no se está empujando
                       if (player->getCurrentAnimationName() == "PUSH_RIGHT")
                            player->setAnimation("STAND_RIGHT");
                    }
                }
                else {
                    // Estem a la dreta: ens movem cap a la dreta el que hem entrat
                    player->setPosition(glm::vec2(pPos.x + rangeCollided.x, pPos.y));

                    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                        if (b->tryPush(-1, 1.0f, pushDist) && (player->getCurrentAnimationName() != "MOVE_LEFT"))
                            player->setAnimation("MOVE_LEFT");
                        else {
                            if (player->getCurrentAnimationName() != "PUSH_LEFT")
                                player->setAnimation("PUSH_LEFT");
                        }
                    }
                    else {
                        // Hay contacto pero no se está empujando
                        if (player->getCurrentAnimationName() == "PUSH_LEFT")
                            player->setAnimation("STAND_LEFT");
                    }
                }
            }
            break;
        }
        case Type::DOOR:
        {
        int centerX = player->getPosition().x + 16.0f;
        if (Game::instance().getKey(GLFW_KEY_UP) && state == NORMAL && rangeCollided.x > 20) {
                
            Door* door = static_cast<Door*>(e);

			if (door->getIsFinalDoor()) {
                if (collectedKeys < allKeys) {
                    cout << "You need to collect all keys to enter the final door!" << endl;
                    break;
                }
            }
                
            // Configurar transición a la nueva habitación
            state = ENTERING_DOOR;
            transitionTimer = 1000.f;
			interactedDoor = door;
            rooms[currentRoom]->setTransitioning(true);

            // Cambiar estado visual
            door->setToVisited();
            player->setAnimation("ENTERING_DOOR");
            player->blockInput(); // Bloquear input del jugador durante la transición
        }
        break;
        }
        case Type::ENEMY:
        {
            Enemy* enemy = static_cast<Enemy*>(e);
            // Si el enemigo se està muriendo no puede matar a nadie
            if (!enemy->isDying() && !godMode) {

                // Configurar transición a la nueva habitación
                state = DYING;
                transitionTimer = 1000.f;
                rooms[currentRoom]->setTransitioning(true);

                // Cambiar estado visual
                player->setAnimation("DIE");
                player->blockInput(); // Bloquear input del jugador durante la transición

				// Reproducir sonido de muerte
				SoundManager::instance().playSound("horse", 0.1);
            }
            break;
        }
        case Type::BULLET:
        {
            Bullet* b = static_cast<Bullet*>(e);
            if (!godMode) {
                // Configurar transición a la nueva habitación
                state = DYING;
                transitionTimer = 1000.f;
                rooms[currentRoom]->setTransitioning(true);

                // Cambiar estado visual
                player->setAnimation("DIE");
                player->blockInput(); // Bloquear input del jugador durante la transición

                // Reproducir sonido de muerte
                SoundManager::instance().playSound("horse", 0.1);

                b->explode();
            }
            break;
        }
        default:
			break;
    }
}

void Level::handleEnemyCollision(Enemy* enemy, Entity* e, glm::vec2& rangeCollided)
{
    switch (e->getType())
    {
        case Type::BARREL:
        {
            Barrel* b = static_cast<Barrel*>(e);
            if (b->isMoving()) {
                // El barril explota en la misma posición del dummy, NO al lado
                glm::vec2 enemyPos = enemy->getPosition();
                b->setPosition(enemyPos);
                enemy->die();
                b->stopPush();
                b->explode();
            }
            else {
                enemy->changeDirection();
                if(enemy->getEnemyType() == EnemyType::CLEVER) {
                    Clever* clever = static_cast<Clever*>(enemy);
                    clever->ignorePlayer();
                }
            }
            break;
        }
        case Type::BULLET:
        {
            Bullet* b = static_cast<Bullet*>(e);
            enemy->die();
            b->explode();
            break;
        }
        default:
            break;
    }
	// Si alguna entity causa distintos efectos en función del tipo de enemigo, ya pondremos un switch dentro de cada caso:
    /*
    switch (enemy->getEnemyType())
    {
        case EnemyType::DUMMY:
        {
        }
    }*/
}

void Level::handleBulletCollision(Bullet* b, Entity* e, glm::vec2& rangeCollided)
{
    switch (e->getType())
    {
        case Type::BARREL:
        {
            Barrel* b = static_cast<Barrel*>(e);
            if (b->isMoving()) b->stopPush();
            b->explode();
            b->explode();
            break;
        }
        default:
            break;

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
        } else if (e->getType() == Type::BARREL) {
            offset = glm::vec2(8.f, 0.f); // Ajustar la X en función de la anchura del sprite definitivo
        } else if (e->getType() == Type::ENEMY) {
            // Custom BoundingBox Dummy
            Enemy* enemy = static_cast<Enemy*>(e);
            switch (enemy->getEnemyType()) {
                case EnemyType::DUMMY:
                    offset = glm::vec2(5.f, 9.f);
					break;
                default:
                    break;
            }
		}

		CollisionInfo collision = overlap(playerBox, e->getBoundingBox(), offset);

        if ((e->getType() != player->getType()) && collision.colliding)
        {
            handlePlayerCollision(e, collision.rangeColision, offset);
        }
		// 2. Colisiones entre y otras entidades (PE: bala y barril)
        Bullet bullet;
        if (e->getType() == bullet.getType())
        {
			Bullet* b = static_cast<Bullet*>(e);
            
            if (b->isExploding() || !b->isActive()) continue;

            for(Entity* e1 : entities) {
                if (!e1->isActive() || e1 == e) continue;
                
                glm::vec2 offset(0.f, 0.f);
                CollisionInfo collisionBullet = overlap(b->getBoundingBox(), e1->getBoundingBox(), offset);

                if (collisionBullet.colliding) {
                    handleBulletCollision(b, e1, collision.rangeColision);
                    break;
                }
			}
            
        }
    }

    // 3. Comprobar colisiones entre los enemigos y las distintas entidades del nivel actual
    vector<Enemy*>& enemies = rooms[currentRoom]->getEnemies();

    for (Enemy* enemy : enemies)
    {
        if (!enemy->isActive()) continue;

        // Ningún enemigo tiene colisiones si se está muriendo
        if (enemy->isDying()) continue;

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
    if(state == NORMAL) checkCollisions();
    player->update(deltaTime);

    switch (state)
    {
    case ENTERING_DOOR:
        transitionTimer = std::max(0.f, transitionTimer - deltaTime);

        if (transitionTimer == 0.f) {
            // Si se han recogido todas las llaves y la puerta interactuada es la puerta final, se completa el nivel
            if (collectedKeys >= allKeys && interactedDoor->getIsFinalDoor()) {
                levelCompleted = true;
            }
            // Sino se entra a la habitación conectada a través de la puerta
            else {
                Door* targetDoor = interactedDoor->getDoorTo();
                currentRoom = targetDoor->getRoom();
                glm::vec2 targetSpawnPosition = targetDoor->getPosition();

                player->setPosition(glm::vec2(targetSpawnPosition.x, targetSpawnPosition.y));
                player->setTileMap(rooms[currentRoom]->getMap());
                player->blockInput();
                player->setAnimation("EXITING_DOOR");
                rooms[currentRoom]->setTransitioning(true);

                // Cambio de estado a EXITING_DOOR
                state = EXITING_DOOR;
                transitionTimer = 1000.f;
            }

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
    default:
		break;
    }

    camera->update(player->getPosition(), rooms[currentRoom]->getMap()->getMapSize() * rooms[currentRoom]->getMap()->getTileSize());

    // Skip content
    if (Game::instance().getKey(GLFW_KEY_G) && releasedG) {
        godMode = !godMode;
        releasedG = false;
        cout << "God mode: " << (godMode ? "ON" : "OFF") << endl;
    }
    else if (!Game::instance().getKey(GLFW_KEY_G)) {
        releasedG = true;
    }
    if (Game::instance().getKey(GLFW_KEY_K)) {
        collectedKeys = allKeys;
	    cout << "collectedKeys: " << collectedKeys << "/" << allKeys << endl;
    }
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