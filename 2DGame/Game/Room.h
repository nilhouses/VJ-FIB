#ifndef _ROOM_INCLUDE
#define _ROOM_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Scene.h"
#include "Entity.h"
#include "Key.h"
#include "Weight.h"
#include "Door.h"
#include "Dummy.h"
#include "Game.h"
#include "Asset.h"

class Room {
public:
    Room(ShaderProgram& shaderProgram, glm::mat4& projection);
    ~Room();

	void init(const glm::ivec2& tileMapPos, const string& levelPath, const string& entityPath, const string& assetPath, int numRoom);
    void update(int deltaTime);
    void render();

    // Getters para que Level pueda consultar cosas si es necesario
	Player* getPlayer() { return player; }
    TileMap* getMap() { return map; }
    vector<Entity*>& getEntities() { return entities; }
    vector<Entity*>& getEnemies() { return enemies; }
    
	void setTransitioning(bool t) { transitioning = t; }


private:
    void loadEntities(const glm::ivec2& tileMapPos, const string& entityPath); // Carga las entidades del nivel a través de un fichero de texto
    void loadAssets(const glm::ivec2& tileMapPos, const string& assetPath); // Carga los elementos decorativos del nivel a través de un fichero de texto
    void createEntity(const glm::ivec2& tileMapPos, const string& type, int tileX, int tileY, int roomTo, int prx, int pry); // Crea una entidad del tipo dado en la posición dada (en tiles)
    void createAsset(const glm::ivec2& tileMapPos, const string& spriteDir, glm::vec2& pos, glm::vec2& size); // Crea un elemento decorativo en la posición dada (en tiles) y con el tamaño dado (en tiles)
   
private:
	Player* player;            // El jugador de la habitación, para facilitar el acceso a la habitación desde el jugador y viceversa
    TileMap* map;
    vector<Entity*> entities;
    vector<Entity*> enemies;
    vector<Asset*> assets;
	int numRoom;
	bool transitioning; // Indica si se está realizando una transición entre habitaciones para evitar actualizar la lógica de la habitación durante la transición

    Camera* camera;
    glm::mat4& projection;

    ShaderProgram& texProgram; // Referencia al shader del Level

};

#endif // _ROOM_INCLUDE