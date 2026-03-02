#ifndef _DUMMY_INCLUDE
#define _DUMMY_INCLUDE

#include "Entity.h"
#include "TileMap.h"


// El dummy contiene su propia información
// Sprite, posición, usa tileMap para detectar colisiones, etc.
class Dummy : public Entity
{
public:
	Dummy();
	~Dummy();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;

	void setTileMap(TileMap* tileMap);
	int getSpeed();
	void incrRight();
	void incrLeft();
	void die();
	bool isDying();

private:
	TileMap* map;						  // Mapa de tiles para detectar colisiones
	bool movingRight;					  // true = right, false = left
	glm::ivec2 size = glm::ivec2(32, 32); // El dummy final tendrá 32, 64?
	bool dying;							  // Para que el dummy solo muera una vez	
	float deathTimer;				      // Para controlar el tiempo que dura la animación de muerte antes del deactivate
};


#endif // _DUMMY_INCLUDE


