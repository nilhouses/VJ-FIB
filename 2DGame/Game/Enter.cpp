#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enter.h"
#include "Game.h"


Enter::Enter(EnterType t) : Entity(Type::ENTER), eType(t) {}

Enter::~Enter()
{
    connectedTo = nullptr;
	if (arrow != nullptr) {
        delete arrow;
        arrow = nullptr;
    }
}

void Enter::update(int deltaTime)
{
    sprite->update(deltaTime);
    if (arrow != nullptr) {
        arrow->update(deltaTime);
    }
}

void Enter::render()
{
    Entity::render();
    if (arrow != nullptr)
        arrow->render();
}

void Enter::setRoomInt(int r) {
	Entity::setRoomInt(r);
	if (arrow != nullptr) arrow->setRoomInt(r);
}

void Enter::setPosition(const glm::vec2& pos) {
	Entity::setPosition(pos);
	if (arrow != nullptr) arrow->setPosition(glm::vec2(pos.x, pos.y));
}