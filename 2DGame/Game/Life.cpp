#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Life.h"
#include "Game.h"


#define PICK_DURATION 450.f // 

enum LifeAnims
{
	IDLE
};

Life::Life() : Entity(Type::LIFE) {}


Life::~Life()
{
	if (sprite != NULL)
		delete sprite;
}


void Life::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f/5.f), c);

	// Atributos de la vida
	collectTimer = 0.0f;

	// Configuraci?n de animaciones
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->changeAnimation(IDLE);
}

void Life::update(int deltaTime)
{
	Entity::update(deltaTime);
    if (collecting) {
        collectTimer += deltaTime;
        if (collectTimer >= PICK_DURATION) this->deactivate();
        return;
    }
}

void Life::collect() {
	// No hacer animaci�n de recoger varias veces
	if (collecting) return;
	collecting = true;
	cout << "Life collected!" << endl;
	// En el update se desactivar� la entidad cuando acabe la animaci�n de explosi�n
}