#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE

#include <GL/glew.h>
#include "Text.h"
#include "Game.h"
#include "HudIcon.h"

#include <iostream>

class Hud
{
public:
	Hud();
	~Hud();

	void init(int numLives, int numKeys);
	void update(int deltaTime, int numLives, int numBullets, float speedBoostDuration, float boostTimeLeft, int collectedKeys);
	void render();

	void setAllKeys(int allKeys) { this->allKeys = allKeys; }

private:
	int numLives;
	int numKeys;
	int allKeys; // Número total de llaves del nivel, para mostrar el contador de llaves como numKeys/allKeys
	int numBullets;
	float speedBoostDuration;
	float boostTimeLeft;

	glm::mat4 hudProj;
	Text text;

	// Iconos
	ShaderProgram hudProgram;

	HudIcon keyIcon;
	HudIcon lifeIcon;
	HudIcon boostIcon;
	HudIcon gunIcon;
};

#endif // _HUD_INCLUDE

