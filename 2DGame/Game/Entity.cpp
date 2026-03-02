#include <GL/glew.h>
#include "Entity.h"
#include "Game.h"
#include <iostream>


Entity::Entity(Type t) : type(t)
{
	sprite = NULL;
}

Entity::~Entity()
{
	if (sprite != NULL)
		delete sprite;
}

void Entity::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spritesheetDir, const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet, Camera* c)
{
	active = true;
	spritesheet.loadFromFile(spritesheetDir, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, sizeInSpritesheet, &spritesheet, &shaderProgram, c);
	tileMapDispl = tileMapPos;
	size = quadSize;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Entity::render()
{
	if (active) sprite->render();
}

void Entity::setPosition(const glm::vec2& pos)
{
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

glm::vec4 Entity::getBoundingBox() const
{
	return glm::vec4(pos.x, pos.y, size.x, size.y);
}



void Entity::printDebugInfo() const
{
	std::cout << "===============================" << std::endl;
	std::cout << "Entity Type: " << static_cast<int>(type) << std::endl;
	std::cout << "In Room: " << room << std::endl;
	std::cout << "Position: (" << pos.x << ", " << pos.y << ")" << std::endl;
	std::cout << "Active: " << (active ? "Yes" : "No") << std::endl;
	std::cout << "Bounding Box: (" << getBoundingBox().x << ", " << getBoundingBox().y << ", " 
			  << getBoundingBox().z << ", " << getBoundingBox().w << ")" << std::endl;
	std::cout << "===============================" << std::endl;
	std::cout << std::endl;
}