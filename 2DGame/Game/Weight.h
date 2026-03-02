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
	void startPush(int dir, int distance);
	void stopPush() { isBeingPushed = false; pushDirection = 0; }
	bool isFalling();
	bool isMoving();
	void explodeWeight();
	bool isExploding() const { return explode; }

private:
	TileMap* map;						// Mapa de tiles para detectar colisiones
	glm::vec2 prevPos;			        // Para trackear caídas(y) y movimiento(x) del peso
	bool explode;						// Para evitar que un peso explote varias veces
	float explosionTimer;				// Para controlar el tiempo que dura la animación de explosión antes del deactivate
	float targetX;						// Donde llega el peso con el último empuje
	bool isBeingPushed;					// Si el peso está siendo empujado
	int pushDirection;					// 1 derecha, -1 izquierda
	float fallSpeed;				    // Para controlar la velocidad de caída del peso, ahora que hay aceleración
};


#endif // _WEIGHT_INCLUDE

