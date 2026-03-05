#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Dummy.h"
#include "Game.h"

// Definimos 4 tipos de animaciones para el dummy
enum DummyAnims
{
    MOVE_LEFT, MOVE_RIGHT, DIE, NUM_ANIMS
};


Dummy::Dummy() : Enemy(EnemyType::DUMMY)
{

}

Dummy::~Dummy()
{
	if (sprite != NULL)
		delete sprite;
}

void Dummy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/dummy.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Atributos característicos del Dummy
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

    sprite->setAnimationSpeed(DIE, 3);
    sprite->addKeyframe(DIE, glm::vec2(0.f, 0.75f));
    sprite->addKeyframe(DIE, glm::vec2(0.25f, 0.75f));
    sprite->addKeyframe(DIE, glm::vec2(0.50f, 0.75f));

	sprite->changeAnimation(MOVE_RIGHT);
}


void Dummy::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (isDying()) {
        deathTimer += deltaTime;
        if (deathTimer >= deathDuration) this->deactivate();
        return;
    }

    bool shouldTurn = false;
    int mapWidth = map->getMapSize().x * map->getTileSize();

    if (movingRight) {
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

    if (shouldTurn) changeDirection();

    // Gravedad
    pos.y += fallStep;
    map->collisionMoveDown(pos, size, &pos.y, fallStep);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Dummy::changeDirection() {
    movingRight = !movingRight;
    if (movingRight) sprite->changeAnimation(MOVE_RIGHT);
    else sprite->changeAnimation(MOVE_LEFT);
}

void Dummy::die()
{
    if (isDying()) return;
    dying = true;
    sprite->changeAnimation(DIE);
    std::cout << "RIP Dummy" << std::endl;
    // En el update se desactivará la entidad cuando acabe la animación de explosión
}