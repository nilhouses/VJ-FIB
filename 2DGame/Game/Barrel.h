#ifndef _BARREL_INCLUDE
#define _BARREL_INCLUDE


#include "Entity.h"
#include "Camera.h"
#include "SoundManager.h"

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
	void startPush(int dir);
	void stopPush() { isBeingPushed = false; pushDirection = 0; }
	bool isFalling();
	bool isMoving();
	void explode();
	bool isExploding() const { return exploding; }
	bool tryPush(int dir, float amount);

private:
	glm::vec2 prevPos;			        // Para trackear ca�das(y) y movimiento(x) del barril
	bool exploding;						// Para evitar que un barril explote varias veces
	float explosionTimer;				// Para controlar el tiempo que dura la animaci�n de explosi�n antes del 
	bool isBeingPushed;					// Si el barril est� siendo empujado
	int pushDirection;					// 1 derecha, -1 izquierda
	float fallSpeed;				    // Para controlar la velocidad de ca�da del barril, ahora que hay aceleraci�n
	// Interacciones con el jugador
	float pushAccumulator = 0.f;
	const float PUSH_THRESHOLD = 30.f; // Tiempo necesario para que el jugador ewmpuje el barril
	bool beingTouchedThisFrame = false;
};


#endif // _BARREL_INCLUDE

