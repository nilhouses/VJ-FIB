#ifndef _ROOM_INCLUDE
#define _ROOM_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Scene.h"
#include "Entity.h"
#include "Enemy.h"
#include "Dummy.h"
#include "Clever.h"
//#include "Shooting.h"
#include "Key.h"
#include "Weight.h"
#include "Door.h"
#include "Game.h"
#include "Asset.h"

class Room {
public:
    Room(ShaderProgram& shaderProgram);
    ~Room();

	void init();
    void update(int deltaTime);
    void render(Camera* camera, glm::mat4& projection);

    TileMap* getMap() { return map; }
	void setMap(TileMap* m) { map = m; }

    vector<Entity*>& getEntities() { return entities; }
	void addEntity(Entity* e) { entities.push_back(e); }

    vector<Enemy*>& getEnemies() { return enemies; }
	void addEnemy(Enemy* e) { enemies.push_back(e); }

	vector<Asset*>& getAssets() { return assets; }
	void addAsset(Asset* a) { assets.push_back(a); }

	void setTransitioning(bool t) { transitioning = t; }

private:
    TileMap* map;
    vector<Entity*> entities;
    vector<Enemy*> enemies;
    vector<Asset*> assets;
	bool transitioning; // Indica si se está realizando una transición entre habitaciones para evitar actualizar la lógica de la habitación durante la transición

    ShaderProgram& texProgram; // Referencia al shader del Level

};

#endif // _ROOM_INCLUDE