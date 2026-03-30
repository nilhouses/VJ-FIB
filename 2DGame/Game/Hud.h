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

	void init(int numLives, int numKeys, int level);
	void update(int deltaTime, int numLives, int numBullets, float speedBoostDuration, float boostTimeLeft, int collectedKeys, bool godMode);
	void render();

	void setAllKeys(int allKeys) { this->allKeys = allKeys; }

private:
	void initShaders();

	int numLives = 0;
	int numKeys = 0;
	int allKeys = 0; // Número total de llaves del nivel, para mostrar el contador de llaves como numKeys/allKeys
	int numBullets = 0;
	float speedBoostDuration = 0.f;
	float boostTimeLeft = 0.f;
	bool godMode = false;

	glm::mat4 hudProj = glm::mat4(1.f);
	Text text = Text();

	// Iconos
	ShaderProgram hudProgram = ShaderProgram(), texProgram = ShaderProgram();

	Texture texBackground = Texture();
	TexturedQuad* background = nullptr;

	HudIcon* keyIcon = nullptr;
	HudIcon* lifeIcon = nullptr;
	HudIcon* boostIcon = nullptr;
	HudIcon* gunIcon = nullptr;
};

#endif // _HUD_INCLUDE

