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
	void ignorePlayer() { ignoringPlayer = true; }

private:
	Player* playerTarget = nullptr; // Puntero al jugador target
	bool isClimbing = false;
	bool climbingUp = false;
	bool climbingDown = false;
	bool ignoringPlayer = false;    // Indica que el Clever tiene que actuar como un Dummy
};


#endif // _CLEVER_INCLUDE