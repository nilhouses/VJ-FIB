#ifndef _CLEVER_INCLUDE
#define _CLEVER_INCLUDE

#include "Enemy.h"
#include "Player.h"

class Clever : public Enemy
{
public:
	Clever();
	~Clever();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
	void die() override;
	void changeDirection() override;

	// Funciones del Clever
	void setPlayerTarget(Player* p) { playerTarget = p; }

private:
	Player* playerTarget = nullptr;
	bool isClimbing = false;
	bool wasInAir = false;
	bool wasClimbing = false;
};


#endif // _CLEVER_INCLUDE