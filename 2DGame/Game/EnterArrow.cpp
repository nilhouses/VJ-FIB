#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "EnterArrow.h"
#include "Game.h"

EnterArrow::EnterArrow() : Entity(Type::ENTERARROW) {}

EnterArrow::~EnterArrow()
{
	if (sprite != NULL)
		delete sprite;
}

void EnterArrow::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/enterArrow.png", glm::ivec2(32, 32), glm::vec2(1.f, 1.f), c);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(1, 1);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(1);
	deactivate();
}

void EnterArrow::update(int deltaTime)
{
	Entity::update(deltaTime);
	deactivate();
}
