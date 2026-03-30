#include "Acid.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"

#define PLATFORM_SPEED 1.5f
#define PAUSE_TIME 0.5f

enum AcidAnims
{
    ACTIVE, NUM_ANIMS
};


Acid::Acid() : Entity(Type::ACID)
{

}

Acid::~Acid() {}

void Acid::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
    // Inicializar los atributos de la Entity
    Entity::init(tileMapPos, shaderProgram, "images/acid.png", glm::ivec2(640, 384), glm::vec2(1.f, 1.f / 3.f), c);

    // Configuración de animaciones
    sprite->setNumberAnimations(NUM_ANIMS);

    sprite->setAnimationSpeed(ACTIVE, 6);
    sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 1.f / 3.f));
    sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 2.f / 3.f));

    sprite->changeAnimation(ACTIVE);
}


void Acid::update(int deltaTime)
{
    sprite->update(deltaTime);
	timer += deltaTime;

	Entity::setPosition(glm::vec2(initPos.x, initPos.y + 96.f * sin(timer / 1000.f)));
}


void Acid::setPosition(const glm::vec2& pos)
{
    Entity::setPosition(pos);
    initPos = pos;
}
