#ifndef _SHOOTER_INCLUDE
#define _SHOOTER_INCLUDE

#include "Enemy.h"
#include "Bullet.h"

class Room; // Forward declaration de Room que sino peta porque Room incluye Shooter y Shooter incluye Room

enum EnemyState {
	WALKING,
	IDLING,
	SHOOTER
};

class Shooter : public Enemy
{
public:
	Shooter();
	~Shooter();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, bool movingRight) override;
	void update(int deltaTime) override;
	void die() override;
	void changeDirection() override;
	void setRoom(Room* room) { currentRoom = room; }

private:
	EnemyState currentState;
	float stateTimer;
	void changeState(EnemyState newState);
	void shoot();

	// Atributos necesarios para crear una bala
	Room* currentRoom = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	Camera* cameraPtr = nullptr;
};


#endif // _SHOOTER_INCLUDE