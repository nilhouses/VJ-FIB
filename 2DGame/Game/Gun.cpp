#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Gun.h"
#include "Game.h"


#define PICK_DURATION 450.f // ms

enum GunAnims
{
	IDLE
};

Gun::Gun() : Entity(Type::GUN) {}


Gun::~Gun()
{
	if (sprite != NULL)
		delete sprite;
}


void Gun::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Atributos de la vida
	collectTimer = 0.0f;

	// Configuraci?n de animaciones
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.25f));
	sprite->changeAnimation(IDLE);
}



void Gun::update(int deltaTime)
{
	Entity::update(deltaTime);
	if (collecting) {
		collectTimer += deltaTime;
		if (collectTimer >= PICK_DURATION) this->deactivate();
		return;
	}
}



void Gun::collect() {
	// No hacer animación de recoger varias veces
	if (collecting) return;
	collecting = true;
	cout << "Gun collected!" << endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}