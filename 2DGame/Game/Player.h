#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Entity.h"


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
	void activateSpeedBoost(float multiplier, int duration);
	bool hasSpeedBoost() { return speedBoostTimer > 0; }

private:
	bool bJumping;						// Indica si el jugador está saltando o no
	int jumpAngle, startY;				// Ángulo de salto y posición inicial en Y para el salto
	bool onGround = false;
	bool blockedInput;
	int itemPickTimer;
	int speedBoostTimer;
	float speedMultiplier = 1.f;
};


#endif // _PLAYER_INCLUDE


