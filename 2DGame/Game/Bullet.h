#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE

#include "Entity.h"
#include "SoundManager.h"

enum class BulletType { PLAYER, ENEMY, NONE };

class Bullet : public Entity
{
public:
    Bullet();
    ~Bullet();
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, BulletType t);
    void update(int deltaTime) override;
	void render() override;
	BulletType getBulletType() const { return t; }

    void setDirection(bool right);
    void explode();
    bool isExploding() const { return exploding; }
    void setPosition(const glm::vec2& pos) override;

private:
	BulletType t = BulletType::NONE;
    Camera* cam = nullptr;
    Sprite* explosionSprite = nullptr;
    bool movingRight = true;
    float speed = 0.f;
    bool exploding = false;
    float explosionTimer = 0.f;
	int distanceTraveled = 0;
    static constexpr float MAX_DISTANCE= 640.f; // px
};

#endif // _BULLET_INCLUDE