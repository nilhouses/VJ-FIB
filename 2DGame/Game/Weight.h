#ifndef _WEIGHT_INCLUDE
#define _WEIGHT_INCLUDE


#include "Entity.h"
#include "TileMap.h"
#include "Camera.h"


class Weight : public Entity
{
public:
	Weight();
	~Weight();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, glm::vec2 prevPos);
	void update(int deltaTime) override;
	void setTileMap(TileMap* tileMap);
	bool incrRight(int units);
	bool incrLeft(int units);
	bool isFalling();
	bool isMoving();
	void explodeWeight();

private:
	TileMap* map;						// Mapa de tiles para detectar colisiones
	glm::vec2 prevPos;			        // Para trackear caídas(y) y movimiento(x) del peso
	bool explode;						// Para evitar que un peso explote varias veces
	float explosionTimer;				// Para controlar el tiempo que dura la animación de explosión antes del deactivate
};


#endif // _WEIGHT_INCLUDE

