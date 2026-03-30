#include "Bullet.h"
#include <iostream>


#define EXPLOSION_DURATION 500.f // ms

enum BulletAnims
{
    STATIC_SPRITE, NUM_ANIMS
};


Bullet::Bullet() : Entity(Type::BULLET) {}

Bullet::~Bullet() {
	if (explosionSprite != nullptr) {
        explosionSprite->free();
        delete explosionSprite;
        explosionSprite = nullptr;
    }
}

void Bullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, BulletType t)
{
    Entity::init(tileMapPos, shaderProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), c);

    speed = 4.0f;
    explosionTimer = 0.0f;
    exploding = false;

    // Configuraci�n de animaciones
    sprite->setNumberAnimations(NUM_ANIMS);
    sprite->setAnimationSpeed(STATIC_SPRITE, 1);

    switch (t)  
    {
        case BulletType::PLAYER:
            sprite->addKeyframe(STATIC_SPRITE, glm::vec2(0.5f, 1.f/5.f));

            break;
        case BulletType::ENEMY:
            sprite->addKeyframe(STATIC_SPRITE, glm::vec2(0.75f, 1.f/5.f));

            break;
        default:
            break;
    }

    explosionSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 1.f / 5.f), &spritesheet, &shaderProgram, c);
    explosionSprite->setNumberAnimations(1);
    explosionSprite->setAnimationSpeed(0, 16);
    explosionSprite->addKeyframe(0, glm::vec2(0.00f, 4.f / 5.f));
    explosionSprite->addKeyframe(0, glm::vec2(0.25f, 4.f / 5.f));
    explosionSprite->addKeyframe(0, glm::vec2(0.50f, 4.f / 5.f));
    explosionSprite->addKeyframe(0, glm::vec2(0.75f, 4.f / 5.f));
    explosionSprite->changeAnimation(0);
    

    sprite->changeAnimation(STATIC_SPRITE);
    cam = c;
    this->t = t;
}

void Bullet::update(int deltaTime)
{

    sprite->update(deltaTime);
    if (exploding) {
        explosionSprite->update(deltaTime);
        explosionTimer += deltaTime;
        if (explosionTimer >= EXPLOSION_DURATION) this->deactivate();
        return;
    }

    if (movingRight) pos.x += static_cast<int>(speed);
    else pos.x -= static_cast<int>(speed);

    distanceTraveled += static_cast<int>(speed);
    if (distanceTraveled >= MAX_DISTANCE) { // Límite de distancia para la bala
        explode();
        return;
	}

    bool collision = false;

    if (movingRight) {
        collision = map->collisionMoveRight(pos, size);
    }
    else {
        collision = map->collisionMoveLeft(pos, size);
    }
    
    if (collision) {
        explode();
        return;
    }

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Bullet::setPosition(const glm::vec2& pos) {
    Entity::setPosition(pos);
	if (cam->isVisible(pos) ||
        cam->isVisible(pos + glm::vec2(MAX_DISTANCE, 0)) ||
        cam->isVisible(pos - glm::vec2(MAX_DISTANCE, 0))) // Si la bala puede llegar al jugador se debería escuchar 
    {   
        if (t == BulletType::PLAYER) SoundManager::instance().playSound("playerShot", 0.4f);
        else SoundManager::instance().playSound("enemyShot", 0.2f);
    }
}

void Bullet::render() {
    if (!active) return;
    if (exploding) explosionSprite->render();
    else Entity::render();
}

void Bullet::explode() {
    // No explotar varias veces
    if (exploding) return;
    exploding = true;
    glm::vec2 centeredPos = glm::vec2(
        tileMapDispl.x + pos.x + 8,  // (32-16)/2
        tileMapDispl.y + pos.y + 8
    );
    explosionSprite->setPosition(centeredPos);
    explosionSprite->changeAnimation(0);

    // En el update se desactivar� la entidad cuando acabe la animaci�n de explosi�n
}

void Bullet::setDirection(bool right) {
	movingRight = right;
}
