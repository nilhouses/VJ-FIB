#ifndef _SHOOTING_INCLUDE
#define _SHOOTING_INCLUDE

#include "Enemy.h"
#include "Bullet.h"

class Room; // Forward declaration de Room que sino peta porque Room incluye Shooting y Shooting incluye Room

enum EnemyState {
	WALKING,
	IDLING,
	SHOOTING
};

class Shooting : public Enemy
{
public:
	Shooting();
	~Shooting();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
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


#endif // _SHOOTING_INCLUDE