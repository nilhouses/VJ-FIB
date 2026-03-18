#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Entity.h"
#include "Bullet.h"

class Room;

// El jugador contiene su propia información
// Sprite, posición, usa tileMap para detectar colisiones, etc.
class Player : public Entity
{
public:
	Player();
	~Player();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;

	void setAnimation(const string& anim);
	string getCurrentAnimationName() const;
	void incrRight();
	void incrLeft();
	void incrUp(int px);
	void blockInput() { blockedInput = true; }
	void unblockInput() { blockedInput = false; }
	void setOnGround(bool b) { onGround = b; }
	bool getOnGround() { return onGround; }
	void handlePush(int dir, bool pushSuccess);
	void pickItem();
	void activateSpeedBoost(float multiplier, float duration);
	bool hasSpeedBoost() { return speedBoostTimer > 0; }
	void addBullet() { numBullets++; }
	int  getBullets() { return numBullets; }
	bool getWasPushing() { return wasPushing; }
	void setWasPushing(bool b) { wasPushing = b; }
	bool hasBullets() { return numBullets > 0; }
	void shoot();
	void setRoom(Room* room) { currentRoom = room; }
	void exitPipe(bool exitingUp);
	void center();
	bool getDeathByMap() { return deathByMap; }

private:
	bool bJumping;						// Indica si el jugador está saltando o no
	int jumpAngle, startY;				// Ángulo de salto y posición inicial en Y para el salto
	bool onGround = false;
	bool blockedInput;
	bool wasPushing = false;				// Indica si el jugador estaba empujando un barril en el frame anterior
	// Atributos necesarios para items
	int itemPickTimer;
	// Atributos necesarios para el SpeedBoost
	float speedBoostTimer;
	float speedMultiplier = 1.f;
	// Atributos necesarios para disparar
	bool isShooting = false;
	float shootAnimTimer;
	float shootCooldown = 0;
	int  numBullets = 0;
	bool facingRight = true;        // Dirección disparo
	Room* currentRoom = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	Camera* cameraPtr = nullptr;
	float startAnimTimer;
	bool deathByMap = false; // Indica si el jugador ha muerto por un daño de mapa (caída, pinchos, etc.) para configurar la animación de muerte correspondiente
};


#endif // _PLAYER_INCLUDE