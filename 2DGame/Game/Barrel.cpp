#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Barrel.h"
#include "Game.h"

#define GRAVITY 0.5f
#define MAX_FALL_SPEED 12.f

#define EXPLOSION_DURATION 1000.f // ms
#define PUSH_SPEED 4.f

enum BarrelAnims
{
	IDLE, START_ROLLING, ROLLING, EXPLOSION, NUM_ANIMS
};

Barrel::Barrel() : Entity(Type::BARREL) {}


Barrel::~Barrel() {}

void Barrel::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, glm::vec2 prevPos)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), c);

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
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 1.f/5.f));
	
	sprite->setAnimationSpeed(START_ROLLING, 16);
	sprite->addKeyframe(START_ROLLING, glm::vec2(0.00f, 3.f/5.f));
	sprite->addKeyframe(START_ROLLING, glm::vec2(0.25f, 3.f/5.f));
	sprite->addKeyframe(START_ROLLING, glm::vec2(0.50f, 3.f/5.f));
	sprite->addKeyframe(START_ROLLING, glm::vec2(0.75f, 3.f/5.f));

	sprite->setAnimationSpeed(ROLLING, 4);
	sprite->addKeyframe(ROLLING, glm::vec2(0.00f, 2.f/5.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.25f, 2.f/5.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.50f, 2.f/5.f));
	sprite->addKeyframe(ROLLING, glm::vec2(0.75f, 2.f/5.f));

	sprite->setAnimationSpeed(EXPLOSION, 4);
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.00f, 4.f/5.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.25f, 4.f/5.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.50f, 4.f/5.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.75f, 4.f/5.f));

	sprite->changeAnimation(IDLE);
	cam = c;
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
		if (startingRoll) {
			// Esperem que acabi START_ROLLING
			if (sprite->animation() != START_ROLLING)
				sprite->changeAnimation(START_ROLLING);
			if (sprite->isLastKeyframe()) {
				startingRoll = false;
				sprite->changeAnimation(ROLLING);
			}
		}
		else {
			if (sprite->animation() != ROLLING)
				sprite->changeAnimation(ROLLING);
		}

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
	map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y, (int)fallSpeed, 0);

	// Si tras caer tocamos el suelo explosión
	if (isFalling() && pos.y == frameStartPos.y) explode();

	// Variables para empujar el barril
	if (!beingTouchedThisFrame && !isBeingPushed) {
		pushAccumulator = max(0.f, pushAccumulator - 1.0f);
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
	startingRoll = true;
	pushDirection = dir;
}

bool Barrel::isFalling() { return pos.y > prevPos.y; }

bool Barrel::isMoving() { return (pos.x != prevPos.x || pos.y != prevPos.y) && (sprite->animation() == ROLLING); }

void Barrel::explode() {
	// No explotar varias veces
	if (exploding) return;
	exploding = true;
	if (cam->isVisible(pos) ||
		cam->isVisible(pos + glm::ivec2(200, 0)) ||
		cam->isVisible(pos - glm::ivec2(200, 0))) // Si el barril está relativamente cerca al campo de visión (200px) de la cámare se debe escuchar
		SoundManager::instance().playSound("explosion", 4.0f);
	sprite->changeAnimation(EXPLOSION);
	cout << "Barrel explosion!" << endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}