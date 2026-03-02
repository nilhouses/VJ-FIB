#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Weight.h"
#include "Game.h"

#define GRAVITY 0.5f
#define MAX_FALL_SPEED 12.f

#define EXPLOSION_DURATION 1000.f // ms
#define PUSH_SPEED 3.f

enum WeightAnims
{
	IDLE, EXPLOSION, NUM_ANIMS
};

Weight::Weight() : Entity(Type::WEIGHT) {}


Weight::~Weight()
{
	if (sprite != NULL)
		delete sprite;
}

void Weight::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, glm::vec2 prevPos)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/weightTileset.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Atributos del peso
	this->prevPos = prevPos;
	explosionTimer = 0.0f;
	explode = false;
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


void Weight::update(int deltaTime)
{
	sprite->update(deltaTime);
	// Si esta haciendo la animación de explosión, no hacer nada más que actualizar la animación
	if (explode) {
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
	if (isFalling() && pos.y == frameStartPos.y) explodeWeight();

	// Actualizaciones de variables
	prevPos = frameStartPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y))); // Actualizar posición visual
}

void Weight::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

bool Weight::incrRight(int units)
{
	pos.x += units;
	// Si detecto colisi�n o se sale del mapa
	if (map->collisionMoveRight(pos, glm::ivec2(32, 32)) || pos.x > ((map->getMapSize().x - 1) * map->getTileSize())){ 
		pos.x -= units;
		return false;
	}
	return true;
}

bool Weight::incrLeft(int units)
{
	pos.x -= units;
	// Si detecto colisi�n o se sale del mapa
	if (map->collisionMoveLeft(pos, glm::ivec2(32, 32)) || pos.x < 0.f) {
		pos.x += units;
		return false;
	}
	return true;
}

void Weight::startPush(int dir, int distance) {
	if (isBeingPushed) return; // Evita empentes infinites
	isBeingPushed = true;
	pushDirection = dir;
	targetX = pos.x + (dir * distance);
}

bool Weight::isFalling() { return pos.y > prevPos.y; }

bool Weight::isMoving() { return (pos.x != prevPos.x || pos.y != prevPos.y); }

void Weight::explodeWeight() {
	// No explotar varias veces
	if (explode) return;
	explode = true;
	sprite->changeAnimation(EXPLOSION);
	std::cout << "BOOM!" << std::endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}