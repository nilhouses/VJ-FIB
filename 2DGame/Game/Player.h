#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

// El jugador contiene su propia información
// Sprite, posición, usa tileMap para detectar colisiones, etc.
class Player
{
public:
	Player();
	~Player();

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
private:
	bool bJumping;						// Si el jugador está saltando
	glm::ivec2 tileMapDispl, posPlayer;	// Posición del jugador en el mapa de tiles
	int jumpAngle, startY;				// Ángulo de salto y posición inicial en Y para el salto
	Texture spritesheet;				// Spritesheet del jugador
	Sprite* sprite;						// Sprite exacto del jugador dentro del spritesheet
	TileMap* map;						// Mapa de tiles para detectar colisiones

};


#endif // _PLAYER_INCLUDE


