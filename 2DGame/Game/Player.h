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
	int getSpeed();
	void incrRight();
	void incrLeft();
	void incrUp(int px);
	void blockInput() { blockedInput = true; }
	void unblockInput() { blockedInput = false; }
	bool isOnGround() const { return onGround; }

private:
	bool bJumping;						// Indica si el jugador está saltando o no
	int jumpAngle, startY;				// Ángulo de salto y posición inicial en Y para el salto
	bool blockedInput;
	bool onGround = true;
};


#endif // _PLAYER_INCLUDE


