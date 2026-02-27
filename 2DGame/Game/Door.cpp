#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Door.h"
#include "Game.h"

enum DoorAnims
{
	CLOSED, OPENED, NUM_ANIMS
};

Door::Door() : Entity(Type::DOOR) {}

Door::~Door()
{
	if (sprite != NULL)
		delete sprite;
}

void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/doors.png", glm::ivec2(32, 64), glm::vec2(0.33333f, 1.f), c);

	visited = false;

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0.33333f, 1.f));

	sprite->setAnimationSpeed(OPENED, 1);
	sprite->addKeyframe(OPENED, glm::vec2(0.f, 1.f));

	sprite->changeAnimation(CLOSED);
}


void Door::update(int deltaTime)
{

}

void Door::setToVisited()
{
	visited = true;
	sprite->changeAnimation(OPENED);
}
