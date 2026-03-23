#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE

#include "Entity.h"
#include "SoundManager.h"

enum class BulletType { PLAYER, ENEMY};

class Bullet : public Entity
{
public:
    Bullet();
    Bullet::~Bullet();
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, BulletType t);
    void update(int deltaTime) override;
	void render() override;
	BulletType getBulletType() const { return t; }

    void setDirection(bool right);
    void explode();
    bool isExploding() const { return exploding; }
    void setPosition(const glm::vec2& pos) override;

private:
	BulletType t;
    Camera* cam = nullptr;
    Sprite* explosionSprite = nullptr;
    bool movingRight = true;
    float speed;
    bool exploding;
    float explosionTimer;
	int distanceTraveled = 0;
    static constexpr float MAX_DISTANCE= 640.f; // px
};

#endif // _BULLET_INCLUDE