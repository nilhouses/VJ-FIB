#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Life.h"
#include "Game.h"


#define PICK_DURATION 450.f // ms

enum LifeAnims
{
	IDLE, COLLECT, NUM_ANIMS
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
	Entity::init(tileMapPos, shaderProgram, "images/Life.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Atributos de la vida
	collectTimer = 0.0f;

	// Configuraci?n de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 3);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(1.f / 3.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(2.f / 3.f, 0.f));

	sprite->setAnimationSpeed(COLLECT, 10);
	sprite->addKeyframe(COLLECT, glm::vec2(0.f, 1.f / 3.f));
	sprite->addKeyframe(COLLECT, glm::vec2(1.f / 3.f, 1.f / 3.f));
	sprite->addKeyframe(COLLECT, glm::vec2(2.f / 3.f, 1.f / 3.f));

	sprite->changeAnimation(IDLE);
}



void Life::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (collecting) {
		collectTimer += deltaTime;
		if (collectTimer >= PICK_DURATION) this->deactivate();
		return;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}



void Life::collect() {
	// No hacer animación de recoger varias veces
	if (collecting) return;
	collecting = true;
	sprite->changeAnimation(COLLECT);
	cout << "Life collected!" << endl;
	// En el update se desactivará la entidad cuando acabe la animación de explosión
}