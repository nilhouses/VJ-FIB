#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "MainMenu.h"
#include "Level.h"
#include "Instructions.h"
#include "Credits.h"
#include "SoundManager.h"


void Game::init()
{
    srand(time(NULL));
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	// Estado inicial del juego
    currentLevel = 1;
	currentScene = nullptr;
	changeState(MAIN_MENU);
    SoundManager::instance().init();
    // Reproducir música de fondo
    SoundManager::instance().playMusic();
}

bool Game::update(int deltaTime)
{
    if (currentScene != nullptr) {
        currentScene->update(deltaTime);
        if (currentScene->getType() == SceneType::LEVEL) {
            Level* level = static_cast<Level*>(currentScene);
            if (level->gameOver()) {
                changeState(MAIN_MENU);
            }
            else {
                if (level->getLevelCompleted()) {
                    currentLevel++;
                    if (currentLevel == 6) changeState(CREDITS); // Ya haremos cinemática tope épica
                    else changeState(PLAYING, currentLevel);
                }
            }
        }
    }

	SoundManager::instance().update(); // Limpiar sonidos que ya han terminado

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (currentScene != nullptr) {
        currentScene->render();
    }
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
    
    if (key == GLFW_KEY_1) {
        currentLevel = 1;
		changeState(PLAYING, currentLevel);
    }
    else if (key == GLFW_KEY_2) {
        currentLevel = 2;
        changeState(PLAYING, currentLevel);
    }
    else if (key == GLFW_KEY_3) {
        currentLevel = 3;
        changeState(PLAYING, currentLevel);
    }
    else if (key == GLFW_KEY_4) {
        currentLevel = 4;
        changeState(PLAYING, currentLevel);
    }
    else if (key == GLFW_KEY_5) {
        currentLevel = 5;
        changeState(PLAYING, currentLevel);
    }
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y) {}
void Game::mousePress(int button) {}
void Game::mouseRelease(int button) {}
void Game::resize(int width, int height) {
    if (currentScene != nullptr) {
        currentScene->resize(width, height, int(SCREEN_WIDTH), int(SCREEN_HEIGHT));
    }
}
bool Game::getKey(int key) const
{
	return keys[key];
}


void Game::changeState(GameState newState, int levelNumber)
{
    currentLevel = levelNumber;

    // Instanciamos la nueva escena seg�n el estado
    switch (newState) {
    case MAIN_MENU:
        currentScene = new MainMenu();
        break;
    case PLAYING:
		cout << "Loading level " << currentLevel << "..." << endl;
        currentScene = new Level(currentLevel);
        break;
    case INSTRUCTIONS:
        currentScene = new Instructions();
        break;
    case CREDITS:
        currentScene = new Credits();
        break;
    }

    // Inicializamos la nueva escena
    currentScene->init();
}



