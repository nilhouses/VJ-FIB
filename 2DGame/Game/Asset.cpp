#include <GL/glew.h>
#include "Asset.h"
#include "Game.h"
#include <iostream>


Asset::Asset()
{
	sprite = NULL;
}

Asset::~Asset()
{
	if (sprite != NULL)
		delete sprite;
}

void Asset::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spritesheetDir, const glm::vec2& quadSize, Camera* c)
{
	spritesheet.loadFromFile(spritesheetDir, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram, c);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Asset::render()
{
	sprite->render();
}

void Asset::setPosition(const glm::vec2& pos)
{
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}