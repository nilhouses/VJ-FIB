#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "EnterArrow.h"
#include "Game.h"

EnterArrow::EnterArrow() : Entity(Type::ENTERARROW) {}

EnterArrow::~EnterArrow() {}

void EnterArrow::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, bool pointsDown)
{
	// Inicializar los atributos de la Entity
	string path = pointsDown ? "images/enterArrowUpsideDown.png" : "images/enterArrow.png";
	Entity::init(tileMapPos, shaderProgram, path, glm::ivec2(32, 32), glm::vec2(1.f, 1.f), c);

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
