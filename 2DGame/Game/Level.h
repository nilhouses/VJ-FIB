#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Scene.h"
#include "Entity.h"
#include "Key.h"
#include "Weight.h"
#include "Door.h"
#include "Game.h"
#include "Asset.h"
#include "Room.h"
#include "Utils.h"

struct CollisionInfo {
	bool colliding;
	glm::vec2 rangeColision;
};

class Level : public Scene
{
public:
    // Pasamos el nivel para el constructor para que sepa que .txt cargar
    Level(int levelNumber);
    ~Level();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
	void initShaders();	// Carga el VS y FS y los linkea al texProgram
	void loadGlobalInfo(const string& globalInfoPath); // Carga la información global del nivel (número total de llaves, etc.) a través de un fichero de texto
	void loadRooms(); // Carga las habitaciones del nivel

	void checkCollisions(); // Comprueba las colisiones entre el jugador y las entidades del nivel actual y actualiza el estado del juego en consecuencia
	void handlePlayerCollision(Player* player, Entity* e, glm::vec2& rangeCollided); // Maneja la colisión entre el jugador y una entidad.
	CollisionInfo overlap(const glm::vec4& a, const glm::vec4& b, const glm::vec2& offset); // Función auxiliar para detectar si dos bounding boxes se solapan (colisionan)

private:
    ShaderProgram texProgram;	// El programa de shaders para renderizar el mapa y el jugador
    float currentTime;			// El tiempo actual del juego

	// Global attributes of the level
    int level;
	int collectedKeys;			// N�mero de llaves recogidas por el jugador
	int allKeys;				// N�mero total de llaves en el nivel
	int numRooms;				// N�mero total de habitaciones en el nivel
	bool levelCompleted;		// Indica si el nivel ha sido completado

	vector<Room*> rooms;			// Las diferentes habitaciones del nivel
	int currentRoom;				// La habitación actual en la que se encuentra el jugador

	int state;
	int targetRoom;					// La habitación a la que se quiere entrar o salir (en caso de estar en estado ENTERING_DOOR o EXITING_DOOR)
	glm::vec2 targetSpawnPosition;	// La posición a la que se quiere posicionar el jugador al entrar o salir por una puerta
};

#endif // _LEVEL_INCLUDE