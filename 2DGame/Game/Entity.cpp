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

void Entity::update(int deltaTime)
{
	if (active) sprite->update(deltaTime);
	if (type == Type::LIFE || type == Type::GUN || type == Type::SPEEDBOOST || type == Type::KEY || type == Type::ENTERARROW) {
		itemTimer += deltaTime;
		float offsetY = sin(itemTimer * 0.003f) * 4.f;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y) + offsetY));
	}

}

void Entity::render()
{
	if (active) {

		if (type == Type::ACID) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			sprite->render();
			glDisable(GL_BLEND);
		}
		else {
			sprite->render();
		}

	}
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
	cout << "===============================" << endl;
	cout << "Entity Type: " << static_cast<int>(type) << endl;
	cout << "In Room: " << room << endl;
	cout << "Position: (" << pos.x << ", " << pos.y << ")" << endl;
	cout << "Active: " << (active ? "Yes" : "No") << endl;
	cout << "Bounding Box: (" << getBoundingBox().x << ", " << getBoundingBox().y << ", " 
			  << getBoundingBox().z << ", " << getBoundingBox().w << ")" << endl;
	cout << "===============================" << endl;
	cout << endl;
}