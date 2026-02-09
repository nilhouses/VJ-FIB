#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

bool Game::update(int deltaTime)
{
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	// [Nil] Canviar el color de fons amb les tecles numèriques('1', '2', '3', ...)
	if (key == GLFW_KEY_1)
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (key == GLFW_KEY_2)
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	if (key == GLFW_KEY_3)
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



