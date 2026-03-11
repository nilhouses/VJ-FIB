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

void Tunnel::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/playerSheet.png", glm::ivec2(32, 64), glm::vec2(1.f / 35.f, 1.f / 7.f), c);

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(UP, 1);
	sprite->addKeyframe(UP, glm::vec2(26.f / 35.f, 2.f / 7.f));

	sprite->setAnimationSpeed(DOWN, 1);
	sprite->addKeyframe(DOWN, glm::vec2(26.f / 35.f, 2.f / 7.f));

	sprite->changeAnimation(UP);
}
