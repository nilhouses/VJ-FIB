#ifndef _DUMMY_INCLUDE
#define _DUMMY_INCLUDE

#include "Enemy.h"

// El dummy contiene su propia información
// Sprite, posición, usa tileMap para detectar colisiones, etc.
class Dummy : public Enemy
{
public:
	Dummy();
	~Dummy();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
	void die() override;
	void changeDirection() override;

};


#endif // _DUMMY_INCLUDE


