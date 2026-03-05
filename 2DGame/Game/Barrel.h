#ifndef _BARREL_INCLUDE
#define _BARREL_INCLUDE


#include "Entity.h"
#include "Camera.h"

class Barrel : public Entity
{
public:
	Barrel();
	~Barrel();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, glm::vec2 prevPos);
	void update(int deltaTime) override;
	bool incrRight(int units);
	bool incrLeft(int units);
	void startPush(int dir, int distance);
	void stopPush() { isBeingPushed = false; pushDirection = 0; }
	bool isFalling();
	bool isMoving();
	void explode();
	bool isExploding() const { return exploding; }

private:
	glm::vec2 prevPos;			        // Para trackear ca�das(y) y movimiento(x) del barril
	bool exploding;						// Para evitar que un barril explote varias veces
	float explosionTimer;				// Para controlar el tiempo que dura la animaci�n de explosi�n antes del deactivate
	float targetX;						// Donde llega el barril con el �ltimo empuje
	bool isBeingPushed;					// Si el barril est� siendo empujado
	int pushDirection;					// 1 derecha, -1 izquierda
	float fallSpeed;				    // Para controlar la velocidad de ca�da del barril, ahora que hay aceleraci�n
};


#endif // _BARREL_INCLUDE

