#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "SpeedBoost.h"
#include "Game.h"


#define PICK_DURATION 450.f // ms

enum SpeedBoostAnims
{
	IDLE, COLLECT, NUM_ANIMS
};

SpeedBoost::SpeedBoost() : Entity(Type::SPEEDBOOST) {}

SpeedBoost::~SpeedBoost()
{
	if (sprite != NULL)
		delete sprite;
}

void SpeedBoost::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/speedboost.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Atributos del boost
	collectTimer = 0.0f;
	// Podríamos leer por fichero los distintos multiplicadores y duraciones de los speedboosts, lo he dejado en el .h como si fuesen todos fijos (que entiendo que lo serán)

	// Configuraci?n de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 3);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(1.f / 3.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(2.f / 3.f, 0.f));

	sprite->setAnimationSpeed(COLLECT, 3);
	sprite->addKeyframe(COLLECT, glm::vec2(0.f, 1.f / 3.f));
	sprite->addKeyframe(COLLECT, glm::vec2(1.f / 3.f, 1.f / 3.f));
	sprite->addKeyframe(COLLECT, glm::vec2(2.f / 3.f, 1.f / 3.f));

	sprite->changeAnimation(IDLE);
}

void SpeedBoost::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (collecting) {
		collectTimer += deltaTime;
		if (collectTimer >= PICK_DURATION) this->deactivate();
		return;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void SpeedBoost::collect() {
	// No hacer animación de recoger varias veces
	if (collecting) return;
	collecting = true;
	sprite->changeAnimation(COLLECT);
	cout << "SpeedBoost collected!" << endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}