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
	IDLE, EXPLOSION, NUM_ANIMS
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
	Entity::init(tileMapPos, shaderProgram, "images/weightTileset.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Atributos del barril
	this->prevPos = prevPos;
	explosionTimer = 0.0f;
	exploding = false;
	isBeingPushed = false;
	pushDirection = 0;
	targetX = 0.0f;
	fallSpeed = 0.0f;

	// Configuraci�n de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	
	sprite->setAnimationSpeed(EXPLOSION, 8);
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.50f, 0.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.0f, 0.25f));

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
		bool collided = false;
		float distRemaining = std::abs(targetX - pos.x);
		int moveAmount = (distRemaining < PUSH_SPEED) ? (int)distRemaining : (int)PUSH_SPEED;

		if (moveAmount > 0) {
			if (pushDirection > 0) collided = !incrRight(moveAmount);
			else collided = !incrLeft(moveAmount);
		}

		// Parar movimiento
		if (collided || distRemaining <= 0.5f || moveAmount <= 0) {
			isBeingPushed = false;
			if (!collided) pos.x = targetX;
		}
	}

	// Caemos, si detectamos colisión abajo, volvemos a subir
	fallSpeed += GRAVITY;
	if (fallSpeed > MAX_FALL_SPEED) fallSpeed = MAX_FALL_SPEED;

	pos.y += fallSpeed;
	map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y, fallSpeed);

	// Si tras caer tocamos el suelo explosión
	if (isFalling() && pos.y == frameStartPos.y) explode();

	// Actualizaciones de variables
	prevPos = frameStartPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y))); // Actualizar posición visual
}

bool Barrel::incrRight(int units)
{
	pos.x += units;
	// Si detecto colisi�n o se sale del mapa
	if (map->collisionMoveRight(pos, glm::ivec2(32, 32)) || pos.x > ((map->getMapSize().x - 1) * map->getTileSize())){ 
		pos.x -= units;
		return false;
	}
	return true;
}

bool Barrel::incrLeft(int units)
{
	pos.x -= units;
	// Si detecto colisi�n o se sale del mapa
	if (map->collisionMoveLeft(pos, glm::ivec2(32, 32)) || pos.x < 0.f) {
		pos.x += units;
		return false;
	}
	return true;
}

void Barrel::startPush(int dir, int distance) {
	if (isBeingPushed) return; // Evita empentes infinites
	isBeingPushed = true;
	pushDirection = dir;
	targetX = pos.x + (dir * distance);
}

bool Barrel::isFalling() { return pos.y > prevPos.y; }

bool Barrel::isMoving() { return (pos.x != prevPos.x || pos.y != prevPos.y); }

void Barrel::explode() {
	// No explotar varias veces
	if (exploding) return;
	exploding = true;
	sprite->changeAnimation(EXPLOSION);
	cout << "Barrel explosion!" << endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}