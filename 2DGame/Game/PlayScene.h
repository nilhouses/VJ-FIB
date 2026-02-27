#ifndef _PLAYSCENE_INCLUDE
#define _PLAYSCENE_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ladder.h"
#include "Scene.h"
#include "Entity.h"
#include "Key.h"
#include "Game.h"

class PlayScene : public Scene
{
public:
    // Pasamos el nivel para el constructor para que sepa que .txt cargar
    PlayScene(int levelNumber);
    ~PlayScene();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();	// Carga el VS y FS y los linkea al texProgram
	void loadEntities(const string& entityPath, Camera* c); // Carga las entidades del nivel a través de un fichero de texto
	void createEntity(const string& type, int tileX, int tileY, Camera* c); // Crea una entidad del tipo dado en la posición dada (en tiles)
	void checkCollisions(); // Comprueba las colisiones entre el jugador y las entidades del nivel y actualiza el estado del juego en consecuencia
	void handlePlayerCollision(Entity* e); // Maneja la colisión entre el jugador y una entidad.

private:
    ShaderProgram texProgram;	// El programa de shaders para renderizar el mapa y el jugador
    float currentTime;			// El tiempo actual del juego

    int level;
    TileMap* map;				// Mapa est�tico
    Player* player;				// Jugador
	vector<Entity*> entities;   // Entidades interactivas del nivel (enemigos, objetos, etc.)
	int collectedKeys;			// N�mero de llaves recogidas por el jugador
	int allKeys;				// N�mero total de llaves en el nivel
	bool levelCompleted;		// Indica si el nivel ha sido completado

};

#endif // _PLAYSCENE_INCLUDE