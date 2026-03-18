#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE

#include "Entity.h"
#include "SoundManager.h"

class Bullet : public Entity
{
public:
    Bullet();
    Bullet::~Bullet();
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
    void update(int deltaTime) override;

    void setDirection(bool right);
    void explode();
    bool isExploding() const { return exploding; }

private:
    bool movingRight = true;
    float speed;
    bool exploding;
    float explosionTimer;
	int distanceTraveled = 0;
};

#endif // _BULLET_INCLUDE