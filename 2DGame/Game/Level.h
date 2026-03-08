#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Scene.h"
#include "Room.h"
#include "Utils.h"
#include "SoundManager.h"

struct CollisionInfo {
	bool colliding;
	glm::vec2 rangeColision;
};

class Level : public Scene
{
public:
    // Pasamos el nivel para el constructor para que sepa que .txt cargar
    Level(int levelNumber, int numLives = 3);
    ~Level();

    void init() override;
    void update(int deltaTime) override;
    void render() override;
	bool gameOver();
	bool getLevelCompleted();
	int getLives() { return numLives; }

private:
	void initShaders();	// Carga el VS y FS y los linkea al texProgram
	void createRooms(); // Carga las habitaciones del nivel

	void checkCollisions(); // Comprueba las colisiones entre el jugador y las entidades del nivel actual y actualiza el estado del juego en consecuencia
	void handlePlayerCollision(Entity* e, glm::vec2& rangeCollided, glm::vec2& offsetUsed); // Maneja la colisión entre el jugador y una entidad.
	void handleEnemyCollision(Enemy* enemy, Entity* e, glm::vec2& rangeCollided); // Maneja la colisión entre un enemigo y una entidad distinta de player
	void handleBulletCollision(Bullet* b, Entity* e, glm::vec2& rangeCollided); // Maneja la colisión entre una bala y otra entidad
	CollisionInfo overlap(const glm::vec4& a, const glm::vec4& b, const glm::vec2& offset); // Función auxiliar para detectar si dos bounding boxes se solapan (colisionan)

	void loadMaps(vector<TileMap*>& maps, int totalMaps);		// Carga los mapas de tiles de cada habitación a través de un fichero de texto
	void loadEntities();	// Carga las entidades del nivel a través de un fichero de texto
	void loadAssets();		// Carga los elementos decorativos del nivel a través de un fichero de texto
	Entity* createEntity(const string& type, int tileX, int tileY, int indexRoom, bool movingRight = false); // Crea una entidad del tipo dado en la posición dada (en tiles)
	void createAsset(const string& spriteDir, glm::vec2& pos, glm::vec2& size, int indexRoom); // Crea un elemento decorativo en la posición dada (en tiles) y con el tamaño dado (en tiles

private:
    ShaderProgram texProgram;	// El programa de shaders para renderizar el mapa y el jugador
    float currentTime;			// El tiempo actual del juego

	// Global attributes of the level
	Player* player;			// El jugador del nivel
    int level;
	int collectedKeys;			// N�mero de llaves recogidas por el jugador
	int allKeys;				// N�mero total de llaves en el nivel
	int numRooms;				// N�mero total de habitaciones en el nivel
	bool levelCompleted;		// Indica si el nivel ha sido completado
	int numLives;
	vector<Room*> rooms;			// Las diferentes habitaciones del nivel
	int currentRoom;				// La habitación actual en la que se encuentra el jugador

	int state;
	Door* interactedDoor = nullptr;
	Entity* interactedEntity = nullptr;

	Camera* camera;
	glm::mat4 projection;


	// Para saltar contenido
	bool godMode = false;
	bool releasedG = true;
};

#endif // _LEVEL_INCLUDE