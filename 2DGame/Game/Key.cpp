#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Key.h"
#include "Game.h"

enum KeyAnims
{
	IDLE, NUM_ANIMS
};

Key::Key() : Entity(Type::KEY){}

Key::~Key()
{
	if (sprite != NULL)
		delete sprite;
}

void Key::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/tilesheet.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.5f));

	sprite->changeAnimation(IDLE);
}


// Esta función solo calcula la posición actual y carga en el sprite la animación correspondiente en cada caso
void Key::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}
