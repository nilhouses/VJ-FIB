#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Door.h"
#include "Game.h"
#include "SoundManager.h"

enum DoorAnims
{
	CLOSED, OPENED, OPENING, NUM_ANIMS
};

Door::Door() : Enter(EnterType::DOOR) {}

Door::~Door()
{
	if (sprite != NULL)
		delete sprite;
}

void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int sr)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/puertas.png", glm::ivec2(32, 64), glm::vec2(1.f / 4.f, 1.f / 13.f), c);

	visited = false;

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, sr / 13.f));
	
	if (sr <= 4 || sr == 6 || sr == 8 || sr >= 11) {
		sprite->setAnimationSpeed(OPENING, 4);
		sprite->addKeyframe(OPENING, glm::vec2(0.f, sr / 13.f));
		sprite->addKeyframe(OPENING, glm::vec2(0.25f, sr / 13.f));
		sprite->addKeyframe(OPENING, glm::vec2(0.5f, sr / 13.f));
		sprite->addKeyframe(OPENING, glm::vec2(0.75f, sr / 13.f));

		sprite->setAnimationSpeed(OPENED, 1);
		sprite->addKeyframe(OPENED, glm::vec2(0.75f, sr / 13.f));
	}
	else {
		sprite->setAnimationSpeed(OPENING, 4);
		sprite->addKeyframe(OPENING, glm::vec2(0.f, sr / 13.f));

		sprite->setAnimationSpeed(OPENED, 1);
		sprite->addKeyframe(OPENED, glm::vec2(0.25f, sr / 13.f));
	}
	spriteRow = sr;
	sprite->changeAnimation(CLOSED);
}

void Door::openingAnim(bool sound) {
	sprite->changeAnimation(OPENING);
	if (sound) {
		if (spriteRow == 0)
			SoundManager::instance().playSound("cowboyDoorOpen", 0.3f);
		else if (spriteRow == 1)
			SoundManager::instance().playSound("clothDoorOpen", 0.3f);
		else
			SoundManager::instance().playSound("doorOpen", 0.3f);
	}
}

void Door::lockedDoorSound() {
	if (spriteRow == 8) SoundManager::instance().playSound("chainDoorLocked", 0.3f);
	else SoundManager::instance().playSound("doorLocked", 0.3f);
}

void Door::openLockedDoorSound() {
	if (spriteRow == 8) SoundManager::instance().playSound("openChainedDoor", 0.5f);
	else SoundManager::instance().playSound("openLockedDoor", 0.5f);
}

void Door::setToVisited()
{
	visited = true;
	sprite->changeAnimation(OPENED);
}
