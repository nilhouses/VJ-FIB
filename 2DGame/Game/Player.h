#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Entity.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

// El jugador contiene su propia información
// Sprite, posición, usa tileMap para detectar colisiones, etc.
class Player : public Entity
{
public:
	Player();
	~Player();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime) override;
	
	void setTileMap(TileMap *tileMap);
	
private:
	bool bJumping;						// Si el jugador está saltando
	int jumpAngle, startY;				// Ángulo de salto y posición inicial en Y para el salto
	TileMap* map;						// Mapa de tiles para detectar colisiones

};


#endif // _PLAYER_INCLUDE


