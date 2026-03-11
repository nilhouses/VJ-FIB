#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enter.h"
#include "Game.h"


Enter::Enter(EnterType t) : Entity(Type::ENTER), eType(t) {}

Enter::~Enter()
{
	if (sprite != NULL)
		delete sprite;
}

void Enter::update(int deltaTime)
{

}