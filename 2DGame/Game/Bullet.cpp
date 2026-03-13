#include "Bullet.h"
#include <iostream>


#define EXPLOSION_DURATION 500.f // ms

enum BulletAnims
{
    LEFT, RIGHT, EXPLODE, NUM_ANIMS
};


Bullet::Bullet() : Entity(Type::BULLET) {}

Bullet::~Bullet()
{
    if (sprite != NULL)
        delete sprite;
}

void Bullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
    Entity::init(tileMapPos, shaderProgram, "images/bullet.png", glm::ivec2(12, 12), glm::vec2(0.25f, 0.25f), c);

    speed = 4.0f;
    explosionTimer = 0.0f;
    exploding = false;

    // Configuraci�n de animaciones
    sprite->setNumberAnimations(NUM_ANIMS);

    sprite->setAnimationSpeed(LEFT, 20);
    sprite->addKeyframe(LEFT, glm::vec2(0.f, 0.25f));
    sprite->addKeyframe(LEFT, glm::vec2(0.5f, 0.25f));
    sprite->addKeyframe(LEFT, glm::vec2(0.25f, 0.25f));

    sprite->setAnimationSpeed(RIGHT, 20);
    sprite->addKeyframe(RIGHT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(RIGHT, glm::vec2(0.5f, 0.f));
    sprite->addKeyframe(RIGHT, glm::vec2(0.75f, 0.25f));

    sprite->setAnimationSpeed(EXPLODE, 20);
    sprite->addKeyframe(EXPLODE, glm::vec2(0.25f, 0.5f));
    sprite->addKeyframe(EXPLODE, glm::vec2(0.5f, 0.5f));
    sprite->addKeyframe(EXPLODE, glm::vec2(0.75f, 0.5f));

    sprite->changeAnimation(RIGHT);
    SoundManager::instance().playSound("shot", (float)0.05);
}

void Bullet::update(int deltaTime)
{

    sprite->update(deltaTime);
    if (exploding) {
        explosionTimer += deltaTime;
        if (explosionTimer >= EXPLOSION_DURATION) this->deactivate();
        return;
    }

    if (movingRight) pos.x += static_cast<int>(speed);
    else pos.x -= static_cast<int>(speed);

    bool collision = false;

    if (movingRight) {
        sprite->changeAnimation(RIGHT);
        collision = map->collisionMoveRight(pos, size);
    }
    else {
        sprite->changeAnimation(LEFT);
        collision = map->collisionMoveLeft(pos, size);
    }
    
    if (collision) {
        explode();
        return;
    }

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Bullet::explode() {
    // No explotar varias veces
    if (exploding) return;
    exploding = true;
    sprite->changeAnimation(EXPLODE);
    cout << "Bullet explosion!" << endl;
    // En el update se desactivar� la entidad cuando acabe la animaci�n de explosi�n
}

void Bullet::setDirection(bool right) {
	movingRight = right;
    auto anim = (movingRight) ? RIGHT : LEFT;
    sprite->changeAnimation(anim);
}
