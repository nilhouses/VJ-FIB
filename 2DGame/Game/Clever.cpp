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

    // Atributos característicos del Clever
    dying = false;
    deathTimer = 0.f;

    // Configuración de animaciones
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

    if (isDying()) {
        deathTimer += deltaTime;
        if (deathTimer >= deathDuration) this->deactivate();
        return;
    }

    if (playerTarget == nullptr) return;

    glm::ivec2 targetPos = playerTarget->getPosition();
    glm::ivec2 targetSize = playerTarget->getSize();
    int playerBottomY = targetPos.y + targetSize.y;
    int cleverBottomY = pos.y + size.y;

    isClimbing = false;

    // 1. Movimiento Vertical
    if (playerBottomY < cleverBottomY && map->collisionLadderUp(pos, size)) {
        pos.y -= speed;
        isClimbing = true;
        if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
    }
    else if (playerBottomY > cleverBottomY && map->collisionLadderDown(pos, size)) {
        pos.y += speed;
        isClimbing = true;
        if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
    }

    // 2. Movimiento Horizontal
    if (!isClimbing) {
        // Misma plataforma que el Player
        if (playerBottomY == cleverBottomY && !ignoringPlayer) {
			// Perseguir horizontalmente al jugador
            if (targetPos.x > pos.x) {
                movingRight = true;
                if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);

                glm::ivec2 nextPos = glm::ivec2(pos.x + speed, pos.y);
                if (!map->collisionMoveRight(nextPos, size)) {
                    pos.x += speed;
                }
            }
            else if (targetPos.x < pos.x) {
                movingRight = false;
                if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);

                glm::ivec2 nextPos = glm::ivec2(pos.x - speed, pos.y);
                if (!map->collisionMoveLeft(nextPos, size)) {
                    pos.x -= speed;
                }
            }
        }
        else {
            // Comportamiento de Dummy
            bool shouldTurn = false;
            int mapWidth = map->getMapSize().x * map->getTileSize();
            if (movingRight) {
                if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
                glm::ivec2 nextPos = glm::ivec2(pos.x + speed, pos.y);
                bool wallAhead = map->collisionMoveRight(nextPos, size);
                bool outOfMap = nextPos.x + size.x >= mapWidth;
                // Pie delantero derecho, 1px de ancho justo debajo del pie
                glm::ivec2 floorCheck = glm::ivec2(nextPos.x + size.x - 1, pos.y + size.y);
                glm::ivec2 floorSize = glm::ivec2(1, 1);
                int tempY = floorCheck.y;
                bool thereIsFloor = map->collisionMoveDown(floorCheck, floorSize, &tempY, 2);

                if (!wallAhead && !outOfMap && thereIsFloor) pos.x += speed;
                else shouldTurn = true;
            }
            else {
                if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
                glm::ivec2 nextPos = glm::ivec2(pos.x - speed, pos.y);
                bool wallAhead = map->collisionMoveLeft(nextPos, size);
                bool outOfMap = nextPos.x <= 0;
                // Pie delantero izquierdo, 1px de ancho justo debajo del pie
                glm::ivec2 floorCheck = glm::ivec2(nextPos.x, pos.y + size.y);
                glm::ivec2 floorSize = glm::ivec2(1, 1);
                int tempY = floorCheck.y;
                bool thereIsFloor = map->collisionMoveDown(floorCheck, floorSize, &tempY, 2);
                if (!wallAhead && !outOfMap && thereIsFloor) pos.x -= speed;
                else shouldTurn = true;
            }

            if (shouldTurn) {
                changeDirection();
                // Si estava actuando como un Dummy significa que ya ha llegado al otro extremo de la plataforma, así que lo volvemos a poner en modo persecución
                if (ignoringPlayer) ignoringPlayer = false;
            }
        }
    }
    // 3. Gravedad
    if (!isClimbing && !map->collisionLadderUp(pos, size) && !map->collisionLadderDown(pos, size)) {
        pos.y += fallStep;
        map->collisionMoveDown(pos, size, &pos.y, fallStep);
    }

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Clever::changeDirection() {
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
    // En el update se desactivará la entidad cuando acabe la animación de explosión
}