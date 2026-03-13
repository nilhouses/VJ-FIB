#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Barrel.h"
#include "Game.h"

#define GRAVITY 0.5f
#define MAX_FALL_SPEED 12.f

#define EXPLOSION_DURATION 1000.f // ms
#define PUSH_SPEED 3.f

enum BarrelAnims
{
	IDLE, ROLLING, EXPLOSION, NUM_ANIMS
};

Barrel::Barrel() : Entity(Type::BARREL) {}


Barrel::~Barrel()
{
	if (sprite != NULL)
		delete sprite;
}

void Barrel::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, glm::vec2 prevPos)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/barrel.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Atributos del barril
	this->prevPos = prevPos;
	explosionTimer = 0.0f;
	exploding = false;
	isBeingPushed = false;
	pushDirection = 0;
	fallSpeed = 0.0f;

	// Configuraci�n de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	
	sprite->setAnimationSpeed(ROLLING, 15);
	sprite->addKeyframe(ROLLING, glm::vec2(0.00f, 0.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.50f, 0.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(EXPLOSION, 8);
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.00f, 0.75f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.25f, 0.75f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.50f, 0.75f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.75f, 0.75f));

	sprite->changeAnimation(IDLE);
}


void Barrel::update(int deltaTime)
{
	sprite->update(deltaTime);
	// Si esta haciendo la animación de explosión, no hacer nada más que actualizar la animación
	if (exploding) {
		explosionTimer += deltaTime;
		if (explosionTimer >= EXPLOSION_DURATION) this->deactivate();
		return;
	}

	// Guardar posición inicial para detectar caídas
	glm::vec2 frameStartPos = pos;

	if (isBeingPushed) {
		if (sprite->animation() != ROLLING) sprite->changeAnimation(ROLLING);

		bool collided = false;
		if (pushDirection > 0) collided = !incrRight((int)PUSH_SPEED);
		else collided = !incrLeft((int)PUSH_SPEED);


		// Parar movimiento
		if (collided) {
			isBeingPushed = false;
			sprite->changeAnimation(IDLE);
		}
	}

	// Caemos, si detectamos colisión abajo, volvemos a subir
	fallSpeed += GRAVITY;
	if (fallSpeed > MAX_FALL_SPEED) fallSpeed = MAX_FALL_SPEED;

	pos.y += (int)(fallSpeed);
	map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y, (int)fallSpeed);

	// Si tras caer tocamos el suelo explosión
	if (isFalling() && pos.y == frameStartPos.y) explode();

	// Variables para empujar el barril
	if (!beingTouchedThisFrame && !isBeingPushed) {
		pushAccumulator = std::max(0.f, pushAccumulator - 1.0f);
	}
	beingTouchedThisFrame = false;

	// Actualizaciones de variables
	prevPos = frameStartPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y))); // Actualizar posición visual
}

bool Barrel::tryPush(int dir, float amount) {
	if (isBeingPushed || exploding) return false;

	beingTouchedThisFrame = true;
	pushAccumulator += amount;

	if (pushAccumulator >= PUSH_THRESHOLD) {
		pushAccumulator = 0;
		startPush(dir);
		return true; // El movimiento se ha iniciado este frame
	}
	return false; // Se está haciendo fuerza para empujar el barril
}

bool Barrel::incrRight(int units)
{
	pos.x += units;

	if (pos.x > (map->getMapSize().x - 1) * map->getTileSize()) {
		this->deactivate();
		return true;
	}
	if (map->collisionMoveRight(pos, getSize())) {
		pos.x -= units;
		return false;
	}
	return true;
}

bool Barrel::incrLeft(int units)
{
	pos.x -= units;

	if (pos.x < 0.f) {
		this->deactivate();
		return true;
	}

	if (map->collisionMoveLeft(pos, getSize())) {
		pos.x += units;
		return false;
	}
	return true;
}

void Barrel::startPush(int dir) {
	if (isBeingPushed) return;
	isBeingPushed = true;
	pushDirection = dir;
}

bool Barrel::isFalling() { return pos.y > prevPos.y; }

bool Barrel::isMoving() { return (pos.x != prevPos.x || pos.y != prevPos.y) && (sprite->animation() == ROLLING); }

void Barrel::explode() {
	// No explotar varias veces
	if (exploding) return;
	exploding = true;
	SoundManager::instance().playSound("explosion", 0.4f);
	sprite->changeAnimation(EXPLOSION);
	cout << "Barrel explosion!" << endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}