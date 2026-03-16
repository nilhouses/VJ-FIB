#ifndef _ASSET_INCLUDE
#define _ASSET_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"


class Asset
{
public:
	Asset();
	~Asset();

public:
	// Un asset solo es un sprite en una posición fija. No tiene lógica de actualización
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spritesheetDir, const glm::vec2& quadSize, Camera* c);
	void render();
	void setPosition(const glm::vec2& pos);

	void setParrallax(int f) { sprite->setParallax(f); }

protected:
	glm::ivec2 tileMapDispl, pos;			// Posición de la entidad en el mapa de tiles
	Texture spritesheet;
	Sprite* sprite;
};

#endif // _ASSET_INCLUDE