#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Tunnel.h"
#include "Game.h"

enum TunnelAnims
{
	UP, DOWN, NUM_ANIMS
};

Tunnel::Tunnel() : Enter(EnterType::TUNNEL) {}

Tunnel::~Tunnel()
{
	if (sprite != NULL)
		delete sprite;
}

void Tunnel::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int sr)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/puertas.png", glm::ivec2(32, 64), glm::vec2(1.f / 4.f, 1.f / 13.f), c);

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(UP, 1);
	sprite->addKeyframe(UP, glm::vec2(0.25f, sr / 13.f));

	sprite->setAnimationSpeed(DOWN, 1);
	sprite->addKeyframe(DOWN, glm::vec2(0.f, sr / 13.f));
}

void Tunnel::setUp()
{
	up = true;
	sprite->changeAnimation(UP);
}

void Tunnel::setDown()
{
	up = false;
	sprite->changeAnimation(DOWN);
}

