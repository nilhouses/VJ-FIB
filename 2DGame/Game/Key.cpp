#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Key.h"
#include "Game.h"

enum KeyAnims
{
	IDLE, NUM_ANIMS
};

Key::Key() : Entity(Type::KEY) {}

Key::~Key()
{
	if (sprite != NULL)
		delete sprite;
}

void Key::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f/5.f), c);

	// Configuraci�n de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(IDLE);
}

void Key::update(int deltaTime)
{
	Entity::update(deltaTime);
}
