#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "MainMenu.h"
#include "Level.h"
#include "Instructions.h"
#include "Credits.h"
#include "LoadingScene.h"
#include "SoundManager.h"
#include "FadeTransition.h"


void Game::startTransition(FadeType type, GameState newState, int levelNumber) {
    if (isTransitioning) return; // Si ya estamos en una transición, no hacemos nada
	currentTransition = new FadeTransition(type, 1000.f);   // 1 segundo de duración para la transición
    pendingState = newState;
    pendingLevel = levelNumber;
    isTransitioning = true;
}


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	// Estado inicial del juego
    currentLevel = 1;
	currentScene = nullptr;
    SoundManager::instance().init();
	changeState(MAIN_MENU);
}

bool Game::update(int deltaTime)
{
    /*
    // Actualizar la transición si existe
    if (currentTransition != nullptr) {
        currentTransition->update(deltaTime);

        // Si la transición ha terminado...
        if (currentTransition->isFinished()) {

            // Si estábamos fundiendo a NEGRO (FADE_OUT), es el momento de cambiar la escena
            if (isTransitioning) {
                changeState(pendingState, pendingLevel);    // Pasamos a la pantalla de carga, ella actualizará al nivel cuando le toque
                isTransitioning = false;

                // lanzamos automáticamente un FADE_IN (negro a transparente)
                delete currentTransition;
                currentTransition = new FadeTransition(FADE_IN, 1000.0f);
            }
            else {
                // Si terminó un FADE_IN, simplemente limpiamos
                delete currentTransition;
                currentTransition = nullptr;
            }
        }
    }
    */

    // Actualizar escena actual
    if (currentScene != nullptr) {
        currentScene->update(deltaTime);

        if (currentScene->getType() == SceneType::LEVEL) {
            Level* level = static_cast<Level*>(currentScene);

            if (level->gameOver()) {
                numLives = 3;
                changeState(MAIN_MENU);
            }
            else {
                if (level->getLevelCompleted()) {
                    currentLevel++;
                    if (currentLevel == 6) changeState(CREDITS); // Ya haremos cinemática tope épica
                    else changeState(LOADING, currentLevel);    //startTransition(FADE_OUT, LOADING, currentLevel);
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

    /*
	// Renderizar la transición por encima de la escena actual
    if (currentTransition != nullptr) {
        currentTransition->render();
	}
    */
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
    bool levelChanged = false;
    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_5) {
        currentLevel = key - GLFW_KEY_0;    // Atajo para cargar niveles directamente
        cout << "Loading level " << currentLevel << endl;
        levelChanged = true;
    }
    if (levelChanged) {
        if (currentScene != nullptr && currentScene->getType() == SceneType::LEVEL) {
            Level* level = static_cast<Level*>(currentScene);
            numLives = level->getLives();
		}
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
        SoundManager::instance().playMusic("menu", true);
        break;
    case PLAYING:
        currentScene = new Level(currentLevel, numLives);
        if (currentLevel <= 2) SoundManager::instance().playMusic("levels1&2", true);
        else if (currentLevel <= 4) SoundManager::instance().playMusic("levels3&4", true);
        else SoundManager::instance().playMusic("level5", true);
        break;
    case INSTRUCTIONS:
        currentScene = new Instructions();
        break;
    case CREDITS:
        currentScene = new Credits();
        break;
    case LOADING:
		cout << "Loading level " << levelNumber << "..." << endl;
        currentScene = new LoadingScene(levelNumber);
		break;
    }

    // Inicializamos la nueva escena
    currentScene->init();
}



