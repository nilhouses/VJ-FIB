#ifndef _PLAYSCENE_INCLUDE
#define _PLAYSCENE_INCLUDE

#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Scene.h"
#include "Entity.h"

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