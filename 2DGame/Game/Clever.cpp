#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Clever.h"
#include "Game.h"

// Definimos 4 tipos de animaciones para el Clever
enum CleverAnims
{
    MOVE_LEFT, MOVE_RIGHT, CLIMB, DIE, NUM_ANIMS
};


Clever::Clever() : Enemy(EnemyType::CLEVER)
{

}

Clever::~Clever()
{
    if (sprite != NULL)
        delete sprite;
}

void Clever::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
    // Inicializar los atributos de la Entity
    Entity::init(tileMapPos, shaderProgram, "images/clever.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

    // Atributos caracter�sticos del Clever
    dying = false;
    deathTimer = 0.f;

    // Configuraci�n de animaciones
    sprite->setNumberAnimations(NUM_ANIMS);

    sprite->setAnimationSpeed(MOVE_LEFT, 20);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.50f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.25f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 20);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.f));

    sprite->setAnimationSpeed(CLIMB, 15);
    sprite->addKeyframe(CLIMB, glm::vec2(0.f, 0.75f));
    sprite->addKeyframe(CLIMB, glm::vec2(0.25f, 0.75f));

    sprite->setAnimationSpeed(DIE, 3);
    sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.75f));
    sprite->addKeyframe(DIE, glm::vec2(0.75f, 0.75f));

    sprite->changeAnimation(MOVE_RIGHT);
}

void Clever::update(int deltaTime)
{
    sprite->update(deltaTime);
    if (isDying() || playerTarget == nullptr) return;

    glm::ivec2 playerPos = playerTarget->getPosition();
    int playerBottomY = playerPos.y + playerTarget->getSize().y;
    int cleverBottomY = pos.y + size.y;

    bool canClimbUp = map->collisionLadderUp(pos, size);
    bool canClimbDown = map->collisionLadderDown(pos, size);

    // 1. Movimiento vertical
    isClimbing = false;
    if (playerBottomY != cleverBottomY) {
        if (playerBottomY < cleverBottomY && canClimbUp) {
            pos.y -= speed;
            isClimbing = true;
        }
        else if (playerBottomY > cleverBottomY && canClimbDown) {
            pos.y += speed;
            isClimbing = true;
        }
        if (isClimbing && sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
    }

    // 2. Movimiento horizontal
    if (!isClimbing) {
        int mapWidth = map->getMapSize().x * map->getTileSize();
        if (movingRight) {
            if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
            glm::ivec2 nextPos(pos.x + speed, pos.y);
            if (!map->collisionMoveRight(nextPos, size) && nextPos.x + size.x < mapWidth)
                pos.x += speed;
            else changeDirection();
        }
        else {
            if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
            glm::ivec2 nextPos(pos.x - speed, pos.y);
            if (!map->collisionMoveLeft(nextPos, size) && nextPos.x > 0)
                pos.x -= speed;
            else changeDirection();
        }
    }

    // 3. Gravedad
    if (!isClimbing) {
        if (!onGround) {
            int oldY = pos.y;
            pos.y += fallStep;
            map->collisionMoveDown(pos, size, &pos.y, fallStep);
            onGround = (pos.y == oldY);
        }
        bool justLanded = (wasInAir && onGround) || (wasClimbing && !isClimbing);
        if (justLanded) movingRight = (playerPos.x > pos.x);
        wasInAir = !onGround;
        onGround = false;
    }
    else {
        wasInAir = false;
        onGround = false;
    }
    wasClimbing = isClimbing;

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Clever::changeDirection() {
    if (isClimbing) return;
    movingRight = !movingRight;
    if (movingRight) sprite->changeAnimation(MOVE_RIGHT);
    else sprite->changeAnimation(MOVE_LEFT);
}

void Clever::die()
{
    if (isDying()) return;
    dying = true;
    sprite->changeAnimation(DIE);
    std::cout << "RIP Clever" << std::endl;
    // En el update se desactivar� la entidad cuando acabe la animaci�n de explosi�n
}