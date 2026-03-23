#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE

#include <GL/glew.h>
#include "Text.h"
#include "Game.h"
#include <iostream>

class Hud
{
public:
	Hud();
	~Hud();

	void init(int numLives, int numKeys);
	void update(int deltaTime, int numLives, int collectedKeys);
	void render();

	void setAllKeys(int allKeys) { this->allKeys = allKeys; }

private:
	int numLives;
	int numKeys;
	int allKeys; // Número total de llaves del nivel, para mostrar el contador de llaves como numKeys/allKeys

	glm::mat4 hudProj;

	Text text;
};

#endif // _HUD_INCLUDE

