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


enum LevelState { NORMAL, ENTERING_DOOR, EXITING_DOOR, DYING, PICKING_OBJECT, ENTERING_PIPE };

// Cooldowns entre interacciones
float transitionTimer = 0.f;


// ---------------------- CONSTRUCTORS ----------------------

Level::Level(int levelNumber, int numLives) : Scene(SceneType::LEVEL)
{
    level = levelNumber;
    this->numLives = numLives;
}

Level::~Level()
{
    if (texProgram.isLinked()) {
        texProgram.free();
    }

    for (Room* room : rooms) {
        if (room != nullptr) {
            delete room;
        }
    }
    rooms.clear();

	for (TileMap* map : maps) {
        if (map != nullptr) {
            delete map;
        }
    }
	maps.clear();

    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
    if (camera != nullptr) {
        delete camera;
        camera = nullptr;
    }
    if (hud != nullptr) {
        delete hud;
        hud = nullptr;
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

Entity* Level::createEntity(const string& type, int tx, int ty, int indexRoom, bool movingRight, bool stay, int rangePixels, int axis, int dir, const vector<glm::ivec2>& segments, int sr)
{
    Entity* entity = nullptr;
    TileMap* map = rooms[indexRoom]->getMap();

    if (type == "PLAYER")
    {
        player = new Player();
        player->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        player->setTileMap(map);
        player->setRoom(rooms[indexRoom]);
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
        door->setTileMap(map);
        door->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera, sr);
        if (indexRoom != 0)
            door->setToVisited();
        entity = door;
    }
    else if (type == "TUNNEL")
    {
        Tunnel* tunnel = new Tunnel();
        tunnel->setTileMap(map);
        tunnel->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera, sr);
        entity = tunnel;
    }
    else if (type == "DUMMY" || type == "CLEVER" || type == "SHOOTER")
    {
        Enemy* enemy = nullptr;
        if (type == "DUMMY") enemy = new Dummy();
        else if (type == "CLEVER") enemy = new Clever();
        else if (type == "SHOOTER") enemy = new Shooter();

        enemy->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera, movingRight);
        enemy->setTileMap(map);

        if (type == "SHOOTER") {
            Shooter* s = static_cast<Shooter*>(enemy);
            s->setRoom(rooms[indexRoom]);
            s->setStay(stay);
        }

        // Añado al vector de enemigos
        rooms[indexRoom]->addEnemy(enemy);
        entity = enemy;
    }
    else if (type == "LIFE")
    {
        Life* life = new Life();
        life->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        entity = life;
    }
    else if (type == "SPEEDBOOST")
    {
        SpeedBoost* sb = new SpeedBoost();
        sb->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        entity = sb;
    }
    else if (type == "GUN")
    {
        Gun* gun = new Gun();
        gun->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
        entity = gun;
    }
    else if (type == "PLATFORM") {
        Platform* p = new Platform();
        p->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera, rangePixels, axis, dir, sr);
        entity = p;
    }
    else if (type == "PIPE") {
        Pipe* pipe = new Pipe();
        pipe->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera, segments, map->getTileSize(), sr);
        entity = pipe;
    }
    else if (type == "ACID") {
		Acid* acid = new Acid();
		acid->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, camera);
		entity = acid;
    }

    // Común para todas las entidades
    if (entity != nullptr)
    {
        if (type != "PIPE") entity->setPosition(glm::vec2(float(tx * map->getTileSize()), float(ty * map->getTileSize())));
        entity->setRoomInt(indexRoom);
        // Añado la entidad a la habitación correspondiente
        if (type != "PLAYER") rooms[indexRoom]->addEntity(entity);
    }

    return entity;
}

void Level::loadEntities()
{
    string entityPath = "levels/level0" + to_string(level) + "/entities.txt";
    ifstream fin(entityPath);

    string type;
    int count;
    bool movingRight = true;

    while (fin >> type)
    {
		//cout << "Loading entity type: " << type << endl;
        fin >> count;

        if (type == "KEY") {
            allKeys = count;
			hud->setAllKeys(allKeys);
        }

        for (int i = 0; i < count; ++i)
        {
            int indexRoom1, tileX1, tileY1;

            if (type == "PIPE") {
                int nSegments, sprite;
                fin >> nSegments >> sprite;
                vector<glm::ivec2> segments;
                for (int i = 0; i < nSegments; ++i)
                {
                    fin >> indexRoom1 >> tileX1 >> tileY1;
                    segments.push_back(glm::ivec2(tileX1, tileY1));
                }
                createEntity(type, tileX1, tileY1, indexRoom1, false, false, 0, 0, 0, segments, sprite);
                continue;
            }

            fin >> indexRoom1 >> tileX1 >> tileY1;

            if (type == "DOOR" || type == "TUNNEL") {
                int indexRoom2, tileX2, tileY2, spriteRow;
                fin >> indexRoom2 >> tileX2 >> tileY2 >> spriteRow;
                bool isFinalDoor = (indexRoom1 == indexRoom2) && (tileX1 == tileX2) && (tileY1 == tileY2); // Si la puerta conecta consigo misma, es la puerta final

                vector<glm::ivec2> segments;
                Entity *e1, *e2;
				Enter* enter1, * enter2;
                
                e1 = createEntity(type, tileX1, tileY1, indexRoom1, false, false, 0, 0, 0, segments, spriteRow);
				enter1 = static_cast<Enter*>(e1);
                if (!isFinalDoor) {
                    e2 = createEntity(type, tileX2, tileY2, indexRoom2, false, false, 0, 0, 0, segments, spriteRow);
                    enter2 = static_cast<Enter*>(e2);

                    // Conecto el túnel o puerta con su contraparte para poder acceder a ella desde la lógica del juego
                    enter1->setConnectedTo(enter2);
                    enter2->setConnectedTo(enter1);

                    // Añado la información extra de la puerta
                    if (type == "TUNNEL") {
                        Tunnel* t1 = static_cast<Tunnel*>(e1);
                        Tunnel* t2 = static_cast<Tunnel*>(e2);

                        t1->setDown();
                        t2->setUp();
                    }
                }
            }
            else if (type == "DUMMY" || type == "CLEVER" || type == "SHOOTER") {
                int dir, stay;
                fin >> dir;
				movingRight = (dir == 1);
                if (type == "SHOOTER") {
                    fin >> stay;
                    createEntity(type, tileX1, tileY1, indexRoom1, movingRight, stay);
                }
				else createEntity(type, tileX1, tileY1, indexRoom1, movingRight);
                
            }
            else if (type == "PLATFORM") {
                int rangePixels, axis, dir, sprite;
                vector<glm::ivec2> segments;

                fin >> rangePixels >> axis >> dir >> sprite;
                createEntity(type, tileX1, tileY1, indexRoom1, false, false, rangePixels, axis, dir, segments, sprite);
            }
            else
                createEntity(type, tileX1, tileY1, indexRoom1);
        }
    }
}

void Level::createAsset(const string& spriteDir, const glm::vec2& pos, const glm::vec2& size, int indexRoom, bool bg)
{
    Asset* asset = new Asset();
    asset->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, spriteDir, size, camera);
    int tileSize = rooms[indexRoom]->getMap()->getTileSize();
    asset->setPosition(glm::vec2(float(pos.x * tileSize), float(pos.y * tileSize)));
    // Añado el asset a la habitación correspondiente
    if (bg) rooms[indexRoom]->setBackground(asset);
    else rooms[indexRoom]->addAsset(asset);
}

void Level::loadAssets()
{
    string assetPath = "levels/level0" + to_string(level) + "/assets.txt";
    ifstream fin(assetPath);

    string path;
    int sizeX, sizeY;
    int indexRoom, tileX, tileY;
    int count;

    int bgCount;
    fin >> bgCount;

    // Backgrounds (un asset por habitación)
    while (bgCount > 0) {
        fin >> path >> count >> sizeX >> sizeY;
        for (int i = 0; i < count; ++i)
        {
            fin >> indexRoom >> tileX >> tileY;
            createAsset(path, glm::vec2(tileX, tileY), glm::vec2(sizeX, sizeY), indexRoom, true);
        }
        bgCount -= count;
    }

    // Resto de Assets
    while (fin >> path)
    {
        fin >> count;

        fin >> sizeX >> sizeY;

        for (int i = 0; i < count; ++i)
        {
            fin >> indexRoom >> tileX >> tileY;

            createAsset(path, glm::vec2(tileX, tileY), glm::vec2(sizeX, sizeY), indexRoom, false);
        }
    }
}

void Level::loadMaps(vector<TileMap*>& maps, int totalMaps)
{
    for (int i = 0; i < totalMaps; ++i)
    {
        string mapPath = "levels/level0" + to_string(level) + "/maps/map" + to_string(i) + ".txt";
        maps[i] = TileMap::createTileMap(mapPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    }
}

void Level::createRooms()
{
    string indexPath = "levels/level0" + to_string(level) + "/indexMapToRoom.txt";
    ifstream fin(indexPath);

    int totalRooms, totalMaps;
    fin >> totalRooms >> totalMaps; // Número de Rooms y Maps distintos

    maps = vector<TileMap*>(totalMaps);
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
        // Recogida de pipes
        vector<Pipe*> roomPipes;
        for (Entity* e : room->getEntities())
            if (e->getType() == Type::PIPE)
                roomPipes.push_back(static_cast<Pipe*>(e));
        // Asignar el player y las pipes a cada clever
        for (Enemy* enemy : room->getEnemies()) {
            if (enemy->getEnemyType() == EnemyType::CLEVER) {
                Clever* clever = static_cast<Clever*>(enemy);
                clever->setPlayerTarget(this->player);
                clever->setPipes(roomPipes);
            }
        }
    }
}

void Level::init()
{
    // Limpieza
    if (texProgram.isLinked()) {
        texProgram.free();
    }

    for (Room* room : rooms) {
        if (room != nullptr) {
            delete room;
        }
    }
    rooms.clear();

    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
    if (camera != nullptr) {
        delete camera;
        camera = nullptr;
    }
    if (hud != nullptr) {
        delete hud;
        hud = nullptr;
    }


    initShaders();

    // Atributos globales del nivel
    collectedKeys = 0;
    string globalInfoPath = "levels/level0" + to_string(level) + "/globalInfo.txt";
    levelCompleted = false;
    rooms = vector<Room*>();
    currentRoom = 0;
    state = NORMAL;

    // Ajustamos la cámara y proyección al tamaño del área de JUEGO (384 de alto)
    camera = new Camera(LEVEL_WIDTH, LEVEL_HEIGHT);
    projection = glm::ortho(0.f, float(LEVEL_WIDTH), float(LEVEL_HEIGHT), 0.f, -100.f, 100.f);

    // Inicializamos el HUD
	hud = new Hud();
    hud->init(numLives, allKeys, level);

    createRooms();
    currentTime = 0.0f;
    SoundManager::instance().setMusicSpeed(1.0f);
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
        float leftMax = max(a.x + offset.x, b.x + offset.x);
        float rightMin = min(a.x + a.z, b.x + b.z);
        float horizontalOverlap = rightMin - leftMax;

        // Calcular rango vertical de colisión
        float topMax = max(a.y + offset.y, b.y + offset.y);
        float bottomMin = min(a.y + a.w, b.y + b.w);
        float verticalOverlap = bottomMin - topMax;

        info.rangeColision = glm::vec2(horizontalOverlap, verticalOverlap);
    }
    else {
        info.rangeColision = glm::vec2(0.f, 0.f);
    }

    return info;
}

void Level::killPlayer() {
    // Configurar transición a la nueva habitación
    state = DYING;
    transitionTimer = 1000.f;
    rooms[currentRoom]->setTransitioning(true);

    // Cambiar estado visual
    player->setAnimation("DIE");
    player->blockInput(); // Bloquear input del jugador durante la transición

    // Reproducir sonido de muerte
    SoundManager::instance().playSound("death", 0.6f);
}

void Level::handlePlayerCollision(Entity* e, glm::vec2& rangeCollided, const glm::vec2& offset, int end = -1) {
    switch (e->getType())
    {
        case Type::KEY:
        {   
            Game::instance().showTutorial("WALK OVER SPACESHIP PARTS TO COLLECT THEM");
            player->pickItem();
            collectedKeys = min(collectedKeys + 1, allKeys);
            // Transición de estado
            state = PICKING_OBJECT;
            transitionTimer = 300.f;
            interactedEntity = e;
            cout << "collectedKeys: " << collectedKeys << "/" << allKeys << endl;
            SoundManager::instance().playSound("key", 1.2f);
            break;
        }

        case Type::LIFE:
        {
            Game::instance().showTutorial("HEARTS KEEP YOU ALIVE LONGER");
            player->pickItem();
            numLives++;
            // Configurar transición de cambio de estado
            state = PICKING_OBJECT;
            transitionTimer = 300.f;
            interactedEntity = e;
            cout << "numLives: " << numLives << endl;
            SoundManager::instance().playSound("life", 0.15f);
            break;
        }

        case Type::SPEEDBOOST:
        {
            Game::instance().showTutorial("BOOSTS WILL MAKE YOU FASTER TEMPORARILY");
            player->pickItem();
            SpeedBoost* sb = static_cast<SpeedBoost*>(e);
            player->activateSpeedBoost(sb->getMultiplier(), sb->getDuration());
            sb->collect();
            // Configurar transición de cambio de estado
            state = PICKING_OBJECT;
            transitionTimer = 300.f;
            interactedEntity = e;
            SoundManager::instance().playSound("kachow", 0.6f);
            SoundManager::instance().setMusicSpeed(1.5f);
            break;
        }

        case Type::GUN:
        {
            Game::instance().showTutorial("PRESS [Q] TO FIRE YOUR WEAPON");
            player->pickItem();
            player->addBullet();
            Gun* gun = static_cast<Gun*>(e);
            gun->collect();
            // Configurar transición de cambio de estado
            state = PICKING_OBJECT;
            transitionTimer = 300.f;
            interactedEntity = e;
            SoundManager::instance().playSound("gun", 0.4f);
            break;
        }

        case Type::BARREL:
        {
            Barrel* b = static_cast<Barrel*>(e);
            if (b->isExploding()) {
                if (!godMode) {
                    killPlayer();
                }
                break;
            }
            else Game::instance().showTutorial("HOLD [LEFT] OR [RIGHT] NEXT TO A BARREL TO PUSH IT");
            
            // Colisiones
            glm::vec2 pPos = player->getPosition();
            glm::vec2 bPos = b->getPosition();

            // Colisión vertical
            if (rangeCollided.y < rangeCollided.x && pPos.y < bPos.y) player->setOnGround(true);
            else
            {
                // Colisión horizontal, lógica de empuje
                if (pPos.x < bPos.x) {
                    player->setPosition(glm::vec2(pPos.x - rangeCollided.x, pPos.y));

                    if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                        player->handlePush(1, b->tryPush(1, 1.0f));
                        player->setWasPushing(true);
                    }
                }
                else {
                    player->setPosition(glm::vec2(pPos.x + rangeCollided.x, pPos.y));

                    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                        player->handlePush(-1, b->tryPush(-1, 1.0f));
                        player->setWasPushing(true);
                    }
                }
            }
            break;
        }

        case Type::PLATFORM:
        {
            Platform* p = static_cast<Platform*>(e);
            glm::ivec2 pSize = p->getSize();
            glm::vec2 pPos = p->getPosition();
            glm::vec2 platformOffset = p->getDeltaMovement();

            glm::ivec2 playerSize = player->getSize();
            glm::vec2 playerPos = player->getPosition();

            // Quitamos 10 px de cada lado de la plataforma para evitar que parezca que el el jugador vuele
            // y que cuando cuando estamos tocando el suelo y llega la plataforma esta nos empuje
            int paddingX = 10;
            float platLimitL = pPos.x + paddingX;
            float platLimitR = pPos.x + pSize.x - paddingX;

            // Colisión Horizontal
            bool isInsideX = (playerPos.x + playerSize.x > platLimitL) && (playerPos.x < platLimitR);

            // Colisión Vertical
            float playerFeet = playerPos.y + playerSize.y;
            bool isAbove = playerFeet <= (pPos.y + 2.0f);

            if (isAbove && isInsideX) {
                
                // Marcar que está en el suelo para evitar que la gravedad lo acelere
                player->setOnGround(true);
                
                // Forzamos la Y del jugador a: (Posición de la plataforma - Altura del jugador)
                float snappedY = pPos.y - playerSize.y;
                
                // Sumamos el delta X para que el jugador se mueva lateralmente con ella
                float movedX = playerPos.x + platformOffset.x;

                player->setPosition(glm::vec2(movedX, snappedY));
            }
            break;
        }
        case Type::ENTER:
        {
            
			Enter* enter = static_cast<Enter*>(e);

            if (enter->getEnterType() == EnterType::DOOR) Game::instance().showTutorial("PRESS [UP] TO  ENTER A DOOR");
			else if (enter->getEnterType() == EnterType::TUNNEL) Game::instance().showTutorial("PRESS [UP] TO TRAVEL THROUGH A TUNNEL");
            
            if (rangeCollided.x > 18 && rangeCollided.y > 56) {
                Enter* enter = static_cast<Door*>(e);
                enter->activateArrow();
                bool isUpPressed = Game::instance().getKey(GLFW_KEY_UP);
                transitionTimer = 1000.f;
                if (isUpPressed && releasedUp && state == NORMAL) {
                    releasedUp = false;
                    // Según el tipo de entrada
                    switch (enter->getEnterType())
                    {
                        case EnterType::DOOR:   // Si es una puerta entonces miramos si la puerta es final y además la animación del jugador es ENTER
                        {
                            Door* door = static_cast<Door*>(enter);

                            bool sound = true;
                            if (door->getIsFinalDoor()) {
                                if (collectedKeys < allKeys) {
                                    if (player->getCurrentAnimationName() != "LOCKED_DOOR") {
                                        Game::instance().showTutorial("YOU MUST COLLECT ALL SPACESHIP PARTS TO REACH THE NEXT LEVEL!");
                                        door->lockedDoorSound();
								        player->setAnimation("LOCKED_DOOR");
                                    }
                                    return;
                                } else {
                                    door->openLockedDoorSound();
                                    sound = false;
                                }
                            }

                            // Cambiar estado visual + sonido (si hace falta)
                            if (!door->getVisited() && !door->isCave() && !door->isWorm()) {
                                door->openingAnim(sound);
                                player->setAnimation("OPEN_AND_ENTER");
                            }
                            else { player->setAnimation("ENTER"); }
                            //if (door->isCave() || door->isWorm() || door->getVisited()) Cualquier puerta 
                            SoundManager::instance().playSound("stepIn", 0.6f);
                            break;
                        }
                        case EnterType::TUNNEL: // Si es un túnel la animación del jugador es ENTER_TUNNEL
                        {
                            Tunnel* tunnel = static_cast<Tunnel*>(enter);
                            SoundManager::instance().playSound("tunnelSteps", 0.8f);
                            if (tunnel->getUp()) player->setAnimation("TUNNEL_ENTER_TOP");
                            else player->setAnimation("TUNNEL_ENTER_BOTTOM");

                            // Configurar cámara para la transición
                            camera->setTotalTimer(transitionTimer);
                            camera->setStartPos(player->getPosition());
                            camera->setEndPos(tunnel->getConnectedTo()->getPosition());

                            //camera->printTransitionInfo();
                        }
                        default:
                            break;
                    }
                    player->center();

                    // Configurar transición a la nueva habitación
                    state = ENTERING_DOOR;
                    interactedEnter = enter;
                    rooms[currentRoom]->setTransitioning(true);
                    player->blockInput(); // Bloquear input del jugador durante la transición

                    break;
                }
            }
            break;
        }

        case Type::PIPE:
        {
            if (state != NORMAL) break;
            Pipe* pipe = static_cast<Pipe*>(e);
            if (pipe->isOccupied()) {
                Game::instance().showTutorial("YOU CAN'T ENTER A PIPE WHEN IT'S OCCUPIED");
                cout << "This pipe is already being used!" << endl;
                if (pipe->getEntryKey(end)) SoundManager::instance().playSound("invalidAction", 0.9f);
                break;
            }
            else {
                Game::instance().showTutorial("USE [UP] OR [DOWN] TO TRAVEL THROUGH A PIPE");
                pipe->activateArrow(end);
            }
            if (pipe->getEntryKey(end)) {
                interactedPipe = pipe;
                player->deactivate();
                player->blockInput();
                pipe->startTransit(end,true);
                state = ENTERING_PIPE;

				// Configurar cámara para la transición
				transitionTimer = pipe->calculateTotalTime();
                camera->setTotalTimer(transitionTimer);
                camera->setStartPos(player->getPosition());
                camera->setEndPos(interactedPipe->getExitPosition((int)player->getSize().y, end));

				//camera->printTransitionInfo();
            }
            break;
        }

        case Type::ENEMY:
        {
            Enemy* enemy = static_cast<Enemy*>(e);
            // Si el enemigo se està muriendo no puede matar a nadie
            if (!enemy->isDying() && !godMode) {
                if (enemy->getEnemyType() == EnemyType::CLEVER) {
                    Clever* clever = static_cast<Clever*>(enemy);
                    if (!clever->isVisible()) break;
                }
                if (enemy->canHurt()) killPlayer();
            }
            break;
        }

        case Type::BULLET:
        {
			cout << "Player just got shot!" << endl;
            Bullet* b = static_cast<Bullet*>(e);
            if (!godMode && !b->isExploding()) {
				killPlayer();
                b->explode();
            }
            break;
        }

        case Type::ACID:
        {
            if (!godMode) killPlayer();
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
        if (b->isMoving() || b->isExploding()) {
            // El barril explota en la misma posición del dummy, NO al lado
            enemy->die();
            b->stopPush();
            b->explode();
        }
        else {
            glm::vec2 enemyPos = enemy->getPosition();
            glm::ivec2 enemySize = enemy->getSize();
            glm::vec2 bPos = b->getPosition();
            glm::ivec2 bSize = b->getSize();

            if (rangeCollided.y < rangeCollided.x && enemyPos.y < bPos.y) enemy->setOnGround(true);
            else {
                if (enemyPos.x < bPos.x) enemy->setPosition(glm::vec2(bPos.x - enemySize.x, enemyPos.y));
                else enemy->setPosition(glm::vec2(bPos.x + bSize.x, enemyPos.y));
                enemy->changeDirection();
            }
        }
        break;
    }   
    case Type::BULLET:
    {
        Bullet* b = static_cast<Bullet*>(e);
        BulletType t = b->getBulletType();
        if (t == BulletType::PLAYER) {
            enemy->die();
            b->explode();
		}
        break;
    }
    case Type::ENTER:
    {
        Enter* enter = static_cast<Enter*>(e);
        if (enter->getEnterType() != EnterType::TUNNEL) break;
        if (enemy->getEnemyType() != EnemyType::CLEVER) break;

        Clever* clever = static_cast<Clever*>(enemy);
        if (clever->isInTunnel() || !clever->isVisible()) break;

        Tunnel* tunnel = static_cast<Tunnel*>(enter);
        glm::vec4 tBox = tunnel->getBoundingBox();

        glm::vec2 cleverPos = clever->getPosition();
        glm::ivec2 cleverSize = clever->getSize();
        float cleverCenterX = cleverPos.x + cleverSize.x * 0.5f;
        float tunnelCenterX = tBox.x + tBox.z * 0.5f;
        float tunnelCenterY = tBox.y + tBox.w * 0.5f;
        float cleverCenterY = cleverPos.y + cleverSize.y * 0.5f;
        bool aligned = abs(cleverCenterX - tunnelCenterX) < 4.f && abs(cleverCenterY - tunnelCenterY) < 2.f;
        if (aligned) clever->notifyTunnelEntry(tunnel);
        break;
    }
        default:
            break;
    }
}

void Level::handleBulletCollision(Bullet* b, Entity* e, glm::vec2& rangeCollided)
{
    switch (e->getType())
    {
    case Type::BARREL:
    {
        Barrel* barrel = static_cast<Barrel*>(e);
        if (barrel->isMoving()) barrel->stopPush();
        barrel->explode();
        b->explode();
        break;
    }
    default:
        break;

    }
}

void Level::handleBarrelCollision(Barrel* b, Entity* e, glm::vec2& rangeCollided)
{
    switch (e->getType())
    {
        case Type::PLATFORM:
            if (b->isMoving()) b->explode();
            break;
        case Type::BARREL:
        {
            Barrel* barrel = static_cast<Barrel*>(e);
            if (barrel->isMoving() || b->isMoving() || barrel->isExploding() || b->isExploding()) {
                barrel->explode(); b->explode();
            }
            break;
        }
        default:
            break;
    }
}

void Level::checkCollisions()
{
    if (!player->isActive()) return;
    // 1. Comprobar colisiones entre el jugador y las entidades del nivel actual
    vector<Entity*>& entities = rooms[currentRoom]->getEntities();

    auto playerBox = player->getBoundingBox();
    // Para que no empuje el barril si ha dejado de estar en contacto con él 
    player->setWasPushing(false);

    for (Entity* e : entities)
    {
        if (!e->isActive()) continue;

        glm::vec2 offset(2.f, 6.f);
        if (e->getType() == Type::KEY || e->getType() == Type::LIFE || e->getType() == Type::SPEEDBOOST || e->getType() == Type::GUN)
            offset = glm::vec2(16.f, 31.9f);
        else if (e->getType() == Type::BARREL)
            offset = glm::vec2(6.f, 4.f);
        else if (e->getType() == Type::BULLET)
            offset = glm::vec2(24.f, 24.f);
        else if (e->getType() == Type::ACID)
			offset = glm::vec2(8.f, 8.f);
        else if (e->getType() == Type::ENEMY) {
            // Custom BoundingBox Dummy
            Enemy* enemy = static_cast<Enemy*>(e);
            switch (enemy->getEnemyType()) {
                case EnemyType::DUMMY:
                    offset = glm::vec2(8.f, 24.f);
                    break;
                case EnemyType::CLEVER:
                    offset = glm::vec2(8.f, 12.f);
                    break;
                case EnemyType::SHOOTER:
                    offset = glm::vec2(8.f, 16.f);
                    break;
                default:
                    break;
            }
        }
        else if (e->getType() == Type::PLATFORM)
            offset = glm::vec2(0.f, -8.f);
        else if (e->getType() == Type::PIPE) { // Se maneja distinto porque una pipe tiene dos zonas de colisión
            Pipe* pipe = static_cast<Pipe*>(e);

            glm::vec4 b0 = pipe->getEndBoundingBox(0);
            glm::vec4 b1 = pipe->getEndBoundingBox(1);

            CollisionInfo c0 = overlap(playerBox, b0, glm::vec2(24.f));
            CollisionInfo c1 = overlap(playerBox, b1, glm::vec2(24.f));

            if (c0.colliding && playerCenteredOn(b0))
                handlePlayerCollision(e, c0.rangeColision, glm::vec2(0.f), 0);
            else if (c1.colliding && playerCenteredOn(b1))
                handlePlayerCollision(e, c1.rangeColision, glm::vec2(0.f), 1);

            continue;
        }

        CollisionInfo collision = overlap(playerBox, e->getBoundingBox(), offset);

        if ((e->getType() != player->getType()) && collision.colliding)
        {
            handlePlayerCollision(e, collision.rangeColision, offset);
        }

        // 2. Colisiones entre entidades
        // 2.1. Bala (para que explote)
        Bullet bullet;
        if (e->getType() == bullet.getType())
        {
            Bullet* b = static_cast<Bullet*>(e);

            if (b->isExploding() || !b->isActive()) continue;

            for (Entity* e1 : entities) {
                if (!e1->isActive() || e1 == e) continue;

                offset = glm::vec2(16.f, 16.f);
                
                CollisionInfo collisionBullet = overlap(b->getBoundingBox(), e1->getBoundingBox(), offset);

                if (collisionBullet.colliding) {
                    handleBulletCollision(b, e1, collision.rangeColision);
                    break;
                }
            }
        }

        // 2.2. Barril (para que explote con la plataforma + colisiones entre barriles)
        Barrel barrel;
        if (e->getType() == barrel.getType())
        {
            Barrel* b = static_cast<Barrel*>(e);

            if (b->isExploding() || !b->isActive()) continue;

            for (Entity* e1 : entities) {
                if (!e1->isActive() || e1 == e) continue;

                glm::vec2 offset(0.f, 0.f);
                CollisionInfo collisionBarrel = overlap(b->getBoundingBox(), e1->getBoundingBox(), offset);

                if (collisionBarrel.colliding) {
                    handleBarrelCollision(b, e1, collision.rangeColision);
                    break;
                }
            }
        }
    }
    if (!player->getWasPushing()) {
        auto anim = player->getCurrentAnimationName();
        if (anim == "PUSH_LEFT" || anim == "PUSH_RIGHT")
            player->setAnimation(player->hasBullets() ? "WEAPON_IDLE" : "IDLE");
    }

    // 3. Comprobar colisiones entre los enemigos y las distintas entidades del nivel actual
    vector<Enemy*>& enemies = rooms[currentRoom]->getEnemies();

    for (Enemy* enemy : enemies)
    {
        if (!enemy->isActive()) continue;
        // Ningún enemigo tiene colisiones si se está muriendo
        if (enemy->isDying()) continue;
        // Al entrar al tubo, el Clever se vuelve invisible y no tiene colisiones
        if (enemy->getEnemyType() == EnemyType::CLEVER) {
            Clever* clever = static_cast<Clever*>(enemy);
            if (!clever->isVisible()) continue;
        }

        auto enemyBox = enemy->getBoundingBox();

        for (Entity* e : entities)
        {
            if (!e->isActive()) continue;

            // Offset de un enemigo con otras entidades (excepto el jugador)
            glm::vec2 offset;
            switch (enemy->getEnemyType()) {
                case EnemyType::DUMMY:
                    offset = glm::vec2(8.f, 28.f);
                    break;
                default:
                    offset = glm::vec2(8.f, 24.f);
                    break;
            }

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

	// Update del Hud con datos actuales
    SpeedBoost* sb = new SpeedBoost();
    float speedBoostDuration = sb->getDuration();
    hud->update(deltaTime, numLives, player->getBullets(), speedBoostDuration, player->getRemainingBoostTime(), collectedKeys, godMode);

    rooms[currentRoom]->update(deltaTime);
    player->update(deltaTime);
    
    // Actualizar la posición de la cámara para que siga al jugador
    camera->update(player->getPosition(), rooms[currentRoom]->getMap()->getMapSize() * rooms[currentRoom]->getMap()->getTileSize());

    switch (state)
    {
        case NORMAL:
        {
            if (player->getDeathByMap()) killPlayer();
            
            checkCollisions();

            break;
        }

        case ENTERING_DOOR:
        {
            transitionTimer = max(0.f, transitionTimer - deltaTime);
            // Gestión de cámara (de inicio a fin)
            if (interactedEnter->getEnterType() == EnterType::TUNNEL) camera->updateTransition(transitionTimer, rooms[currentRoom]->getMap()->getMapSize() * rooms[currentRoom]->getMap()->getTileSize());

            if (transitionTimer == 0.f) {

                // Si se ha entrado por la puerta final se completa el nivel
                if (interactedEnter->getEnterType() == EnterType::DOOR) {
                    Door* interactedDoor = static_cast<Door*>(interactedEnter);
                    interactedDoor->setToVisited();
                    if (collectedKeys >= allKeys && interactedDoor->getIsFinalDoor()) {
                        levelCompleted = true;
                        break;
                    }
                }

                // Si se ha entrado por cualquier ENTER diferente a la puerta final
                Enter* targetEnter = interactedEnter->getConnectedTo();
                currentRoom = targetEnter->getRoom();

                glm::vec2 targetSpawnPosition = targetEnter->getPosition();

                player->setPosition(glm::vec2(targetSpawnPosition.x, targetSpawnPosition.y));
                player->setTileMap(rooms[currentRoom]->getMap());
                player->blockInput();
                rooms[currentRoom]->setTransitioning(true);

                // Según el tipo de entrada, se configura la animación del jugador
                switch (interactedEnter->getEnterType())
                {
                case EnterType::DOOR:
                    (player->hasBullets()) ? player->setAnimation("WEAPON_IDLE") : player->setAnimation("IDLE");
                    transitionTimer = 200.f;
                    break;
                case EnterType::TUNNEL:
                {
                    Tunnel* tunnel = static_cast<Tunnel*>(interactedEnter);
                    transitionTimer = 1000.f;
                    if (tunnel->getUp()) player->setAnimation("TUNNEL_LEAVE_TOP");
                    else player->setAnimation("TUNNEL_LEAVE_BOTTOM");
                    break;
                }
                default:
                    break;
                }

                // Cambio de estado a EXITING_DOOR
                state = EXITING_DOOR;
            }

            break;
        }

        case EXITING_DOOR:
        {
            transitionTimer = max(0.f, transitionTimer - deltaTime);

            if (transitionTimer == 0.f) {
                if (player->getCurrentAnimationName() != "IDLE" && player->getCurrentAnimationName() != "WEAPON_IDLE")
                    (player->hasBullets()) ? player->setAnimation("WEAPON_IDLE") : player->setAnimation("IDLE");
                player->unblockInput();
                rooms[currentRoom]->setTransitioning(false);
                state = NORMAL;
            }

            break;
        }

        case ENTERING_PIPE:
        {
            transitionTimer = max(0.f, transitionTimer - deltaTime);
            // Gestión de cámara (de inicio a fin)
            camera->updateTransition(transitionTimer, rooms[currentRoom]->getMap()->getMapSize() * rooms[currentRoom]->getMap()->getTileSize());

            if (interactedPipe->isTransitComplete()) {
                bool exitingUp = interactedPipe->isExitingUp();
                glm::vec2 exitPos = interactedPipe->getExitPosition((int)player->getSize().y);
                interactedPipe = nullptr;

                player->setPosition(exitPos);
                player->activate();
                player->exitPipe(exitingUp); // bloquea input y pone startAnimTimer
                state = NORMAL;
            }
            break;
        }

        case DYING:
        {
            transitionTimer = max(0.f, transitionTimer - deltaTime);
            if (transitionTimer == 0.f) {
                // Animación acabada
                player->unblockInput();
                if (numLives > 0) {
                    numLives--;
                    cout << "numLives: " << numLives << endl;
                    init(); // Temporal, el init vuelve a leer todos los ficheros. Necesitaremos un reset()
                    return;
                }
            }

            break;
        }

        case PICKING_OBJECT:
        {
            transitionTimer = max(0.f, transitionTimer - deltaTime);
            if (transitionTimer == 0.f) {
                // Animación acabada
                player->unblockInput();
                interactedEntity->deactivate();
                state = NORMAL;
            }

            break;
        }

        default:
            break;
    }


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
    if (!Game::instance().getKey(GLFW_KEY_UP)) {
        releasedUp = true;
    }

    /*if (Game::instance().getKey(GLFW_KEY_F)) {
        player->activateSpeedBoost(2.f, 600000.f);
    }*/
}


// ---------------------- RENDER ----------------------

void Level::render()
{
    // Calculamos cuánto mide el HUD y el nivel proporcionalmente al tamaño actual de la ventana
    float hudRatio = (float)HUD_HEIGHT / (float)SCREEN_HEIGHT;
    int scaledHudHeight = (int)(viewHeight * hudRatio);
    int scaledLevelHeight = viewHeight - scaledHudHeight;

    // HUD
    // Empezamos en la Y base del contenedor (viewY)
    glViewport(viewX, viewY, viewWidth, scaledHudHeight);
    hud->render();

    // LEVEL
    // Empezamos donde termina el HUD (viewY + scaledHudHeight)
    glViewport(viewX, viewY + scaledHudHeight, viewWidth, scaledLevelHeight);
    rooms[currentRoom]->render(camera, projection);
    player->render();

    // RESET
    glViewport(viewX, viewY, viewWidth, viewHeight);
}

bool Level::gameOver() { return (numLives == 0); }

bool Level::getLevelCompleted() { return levelCompleted; } // Se deberá poner que se haya entrado en la última puerta, con todas las llaves recogidas

bool Level::playerCenteredOn(const glm::vec4& bbox) const {
    glm::vec2 pPos = player->getPosition();
    glm::ivec2 pSize = player->getSize();
    float centerX = pPos.x + pSize.x * 0.5f;
    float centerY = pPos.y + pSize.y * 0.5f;

    return centerX >= bbox.x && centerX <= bbox.x + bbox.z && centerY >= bbox.y && centerY <= bbox.y + bbox.w;
}