#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Weight.h"
#include "Game.h"

#define FALL_STEP 6	

enum WeightAnims
{
	IDLE, NUM_ANIMS
};

Weight::Weight() : Entity(Type::WEIGHT) {}


Weight::~Weight()
{
	if (sprite != NULL)
		delete sprite;
}

void Weight::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/tiles2.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(IDLE);
}


// Esta función solo calcula la posición actual y carga en el sprite la animación correspondiente en cada caso
void Weight::update(int deltaTime)
{
	sprite->update(deltaTime);
	pos.y += FALL_STEP;
	map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y, FALL_STEP);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Weight::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

bool Weight::incrRight(int units)
{
	pos.x += units;
	// Si detecto colisión o se sale del mapa
	if (map->collisionMoveRight(pos, glm::ivec2(32, 32)) || pos.x > ((map->getMapSize().x - 1) * map->getTileSize())){ 
		pos.x -= units;
		return false;
	}
	return true;
}

bool Weight::incrLeft(int units)
{
	pos.x -= units;
	// Si detecto colisión o se sale del mapa
	if (map->collisionMoveLeft(pos, glm::ivec2(32, 32)) || pos.x < 0.f) {
		pos.x += units;
		return false;
	}
	return true;
}