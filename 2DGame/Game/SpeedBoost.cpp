#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "SpeedBoost.h"
#include "Game.h"


#define PICK_DURATION 450.f // ms

enum SpeedBoostAnims
{
	IDLE
};

SpeedBoost::SpeedBoost() : Entity(Type::SPEEDBOOST) {}

SpeedBoost::~SpeedBoost() {}

void SpeedBoost::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), c);

	// Atributos del boost
	collectTimer = 0.0f;
	// Podríamos leer por fichero los distintos multiplicadores y duraciones de los speedboosts, lo he dejado en el .h como si fuesen todos fijos (que entiendo que lo serán)

	// Configuraci?n de animaciones
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	sprite->changeAnimation(IDLE);
}


void SpeedBoost::update(int deltaTime)
{
	Entity::update(deltaTime);
	if (collecting) {
		collectTimer += deltaTime;
		if (collectTimer >= PICK_DURATION) this->deactivate();
		return;
	}
}

void SpeedBoost::collect() {
	// No hacer animación de recoger varias veces
	if (collecting) return;
	collecting = true;
	cout << "SpeedBoost collected!" << endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}