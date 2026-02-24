#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

// Esta clase contiene todas las entidades de un nivel y es responsable de actualizarlas y renderizarlas.
class Scene
{

public:
	Scene();
	~Scene();

	void init();					// Muestra el mapa y el jugador en la posición inicial
	void update(int deltaTime);		// Actualiza el mapa y el jugador (en este caso solo al jugador, mapa estático)	
	void render();					// Pinta el mapa y el jugador
	
private:
	void initShaders();	// Carga el VS y FS y los linkea al texProgram

private:
	TileMap* map;				// El mapa de tiles del nivel
	Player* player;				// El jugador
	ShaderProgram texProgram;	// El programa de shaders para renderizar el mapa y el jugador
	float currentTime;			// El tiempo actual del juego
	glm::mat4 projection;		// La matriz de proyección para renderizar el mapa y el jugador

};


#endif // _SCENE_INCLUDE

