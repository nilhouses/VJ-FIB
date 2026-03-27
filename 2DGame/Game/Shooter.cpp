#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Shooter.h"
#include "Room.h"

// Definimos 4 tipos de animaciones para el Shooter
enum ShooterAnims
{
    MOVE_LEFT, MOVE_RIGHT, IDLE_LEFT, IDLE_RIGHT, SHOOT_LEFT, SHOOT_RIGHT, DIE, NUM_ANIMS
};

Shooter::Shooter() : Enemy(EnemyType::SHOOTER) {
    sprite = nullptr;
}

Shooter::~Shooter()
{
    if (sprite != NULL)
        delete sprite;
}

void Shooter::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, bool movingRight)
{
    this->shaderProgram = &shaderProgram;
    this->cameraPtr = c;

    // Inicializar los atributos de la Entity
    Entity::init(tileMapPos, shaderProgram, "images/ShooterEnemy.png", glm::ivec2(32, 64), glm::vec2(0.25f, 1.f/5.f), c);

    // Atributos caracter�sticos deShooterng
    dying = false;
    deathTimer = 0.f;

    // Configuraci�n de animaciones
    sprite->setNumberAnimations(NUM_ANIMS);

    sprite->setAnimationSpeed(MOVE_LEFT, 4);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.00f, 1.f/5.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 1.f/5.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.50f, 1.f/5.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 1.f/5.f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 4);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.f));

    sprite->setAnimationSpeed(SHOOT_LEFT, 4);
    sprite->addKeyframe(SHOOT_LEFT, glm::vec2(0.00f, 4.f/5.f));
    sprite->addKeyframe(SHOOT_LEFT, glm::vec2(0.25f, 4.f/5.f));
    sprite->addKeyframe(SHOOT_LEFT, glm::vec2(0.50f, 4.f/5.f));
    sprite->addKeyframe(SHOOT_LEFT, glm::vec2(0.75f, 4.f/5.f));

    sprite->setAnimationSpeed(IDLE_LEFT, 1);
    sprite->addKeyframe(IDLE_LEFT, glm::vec2(0.00f, 4.f/5.f));

    sprite->setAnimationSpeed(IDLE_RIGHT, 1);
    sprite->addKeyframe(IDLE_RIGHT, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(SHOOT_RIGHT, 4);
    sprite->addKeyframe(SHOOT_RIGHT, glm::vec2(0.00f, 3.f/5.f));
    sprite->addKeyframe(SHOOT_RIGHT, glm::vec2(0.25f, 3.f/5.f));
    sprite->addKeyframe(SHOOT_RIGHT, glm::vec2(0.50f, 3.f/5.f));
    sprite->addKeyframe(SHOOT_RIGHT, glm::vec2(0.75f, 3.f/5.f));

    sprite->setAnimationSpeed(DIE, 8);
    sprite->addKeyframe(DIE, glm::vec2(0.f, 2.f/5.f));
    sprite->addKeyframe(DIE, glm::vec2(0.25f, 2.f/5.f));
    sprite->addKeyframe(DIE, glm::vec2(0.50f, 2.f/5.f));
    sprite->addKeyframe(DIE, glm::vec2(0.75f, 2.f/5.f));


    this->movingRight = movingRight;
	changeState(WALKING);
}


void Shooter::setStay(bool s) {
    stay = s;
	if (stay) changeState(IDLING);
}


void Shooter::changeState(EnemyState newState) {
    currentState = newState;
    switch (currentState) {
    case WALKING:
        stateTimer = 2000.f + (float)(rand() % 2000); // Cada (2-4) segundos dispara
        sprite->changeAnimation(movingRight ? MOVE_RIGHT : MOVE_LEFT);
        break;
    case IDLING:
        stateTimer = 2000.f + (float)(rand() % 2000); // Cada (2-4) segundos dispara
        sprite->changeAnimation(movingRight ? IDLE_RIGHT : IDLE_LEFT);
        break;
    case RELOAD:
        stateTimer = 500.f; // 0.5 segundos preparando el disparo
        sprite->changeAnimation(movingRight ? IDLE_RIGHT : IDLE_LEFT);
        break;
    case SHOOTING:
        stateTimer = 1000.f;  // Disparo de 1 segundo y medio
        sprite->changeAnimation(movingRight ? SHOOT_RIGHT : SHOOT_LEFT);
        shoot();
        break;
    }
}


void Shooter::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (isDying()) {
        deathTimer += deltaTime;
        if (deathTimer >= deathDuration) this->deactivate();
        return;
    }

    stateTimer -= deltaTime;

    if (currentState == WALKING) {
        bool shouldTurn = false;
        int mapWidth = map->getMapSize().x * map->getTileSize();

        if (movingRight) {
            glm::ivec2 nextPos = glm::ivec2(pos.x + speed, pos.y);
            bool wallAhead = map->collisionMoveRightEnemy(nextPos, size);
            bool outOfMap = nextPos.x + size.x >= mapWidth;

            // Pie delantero derecho, 1px de ancho justo debajo del pie
            glm::ivec2 floorCheck = glm::ivec2(nextPos.x + size.x - 1, pos.y + size.y);
            int tempY = floorCheck.y;
            bool thereIsFloor = map->collisionMoveDown(floorCheck, glm::ivec2(1, 1), &tempY, 2);

            if (!wallAhead && !outOfMap && thereIsFloor) pos.x += speed;
            else shouldTurn = true;
        }
        else {
            glm::ivec2 nextPos = glm::ivec2(pos.x - speed, pos.y);
            bool wallAhead = map->collisionMoveLeftEnemy(nextPos, size);
            bool outOfMap = nextPos.x <= 0;

            // Pie delantero izquierdo, 1px de ancho justo debajo del pie
            glm::ivec2 floorCheck = glm::ivec2(nextPos.x, pos.y + size.y);
            int tempY = floorCheck.y;
            bool thereIsFloor = map->collisionMoveDown(floorCheck, glm::ivec2(1, 1), &tempY, 2);

            if (!wallAhead && !outOfMap && thereIsFloor) pos.x -= speed;
            else shouldTurn = true;
        }

        if (shouldTurn && onGround) changeDirection();
        if (stateTimer <= 0) changeState(RELOAD);

    }
    else if (currentState == IDLING) {
        if (stateTimer <= 0) changeState(RELOAD);
	}
    else if (currentState == RELOAD) {
        if (stateTimer <= 0) changeState(SHOOTING);
    }
    else if (currentState == SHOOTING) {
        if (stateTimer <= 0) {
            if (stay) changeState(IDLING);
			else changeState(WALKING);
        }
    }
    // Gravedad
    int prevY = pos.y;
    pos.y += fallStep;
    map->collisionMoveDown(pos, size, &pos.y, fallStep);
    onGround = (pos.y < prevY + fallStep);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Shooter::changeDirection() {
    movingRight = !movingRight;
    if (movingRight) sprite->changeAnimation(MOVE_RIGHT);
    else sprite->changeAnimation(MOVE_LEFT);
}

void Shooter::shoot() {
    if (currentRoom == nullptr) {
        cout << "The shooting enemy doesn't have a room assigned" << endl; 
        return;
    }

    Bullet* bullet = new Bullet();
    bullet->init(glm::ivec2(tileMapDispl.x, tileMapDispl.y), *shaderProgram, cameraPtr, BulletType::ENEMY);
	bullet->setDirection(movingRight);
    
    glm::ivec2 enemySize = this->getSize();
    glm::ivec2 bulletSize = bullet->getSize();

    glm::vec2 bulletPos;
    
	float gap = 2.f; // Separaci�n horizontal entre el enemigo y la bala

    if (movingRight) bulletPos.x = this->pos.x + enemySize.x + gap;
    else bulletPos.x = this->pos.x - bulletSize.x - gap;
    
    bulletPos.y = float(this->pos.y + (5.f/16.f * enemySize.y));

    bullet->setPosition(bulletPos);
    bullet->setTileMap(map);
    bullet->setDirection(movingRight);
    currentRoom->addEntity(bullet);
}

void Shooter::die()
{
    if (isDying()) return;
    dying = true;
    sprite->changeAnimation(DIE);
    cout << "RIP Shooter" << endl;
    // En el update se desactivar� la entidad cuando acabe la animaci�n de explosi�n
}