#include "Hud.h"


Hud::Hud()
{

}

Hud::~Hud()
{

}

void Hud::init(int numLives, int numKeys)
{
	this->numLives = numLives;
	this->numKeys = numKeys;

	if (!text.init("fonts/PressStart2P.ttf"))
		std::cout << "Could not load font for Loading Scene!" << std::endl;

	hudProj = glm::ortho(0.f, float(HUD_WIDTH), float(HUD_HEIGHT), 0.f);
}

void Hud::update(int deltaTime, int numLives, int collectedKeys)
{
	this->numLives = numLives;
	this->numKeys = collectedKeys;
}

void Hud::render()
{
	// preparamos strings
	std::string livesStr = "LIVES: " + std::to_string(numLives);
	std::string keysStr = "KEYS: " + std::to_string(numKeys) + "/" + std::to_string(allKeys);

	glUseProgram(0);

	float textY = 45.f;

	// Vidas a la izquierda
	text.render(livesStr, glm::vec2(40, textY), 24, glm::vec4(1, 1, 1, 1), hudProj);

	// Llaves a la derecha
	text.render(keysStr, glm::vec2(390, textY), 24, glm::vec4(1, 1, 0.2f, 1), hudProj);
}