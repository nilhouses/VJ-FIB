#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "MainMenu.h"
#include "Level.h"
#include "Instructions.h"
#include "Comic.h"
#include "Credits.h"
#include "LoadingScene.h"
#include "SoundManager.h"
#include "GameOver.h"

void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	// Estado inicial del juego
    currentLevel = 1;
	currentScene = nullptr;
    SoundManager::instance().init();
    changeState(MAIN_MENU);

	// Texto para los mensajes de info
    if (!tutorialText.init("fonts/PressStart2P.ttf"))
        cout << "Could not load tutorial font!!!" << endl;
    actionMsg.init(&tutorialText);
    uiProjection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	SoundManager::instance().setMasterVolume(1.f);
}

bool Game::update(int deltaTime)
{
    // Version compatible Web
    int fixedDeltaTime = (deltaTime > 17) ? 17 : deltaTime;

    SoundManager::instance().update(); // Limpiar sonidos que ya han terminado
    actionMsg.update(fixedDeltaTime);

    // Actualizar escena actual
    if (currentScene != nullptr) {
        SceneType type = currentScene->getType();

        currentScene->update(fixedDeltaTime);

        if (type == SceneType::COMIC) {
            Comic* c = static_cast<Comic*>(currentScene);
            if (c->hasFinished()) {
                if (c->isInitial()) {
                    try {
                        changeState(LOADING, 1);
                    }
                    catch (...) {
                        cout << "Error" << endl;
                    }
                }
                else changeState(CREDITS);
            }
        }
        else if (type == SceneType::LEVEL) {
            Level* level = static_cast<Level*>(currentScene);

            if (level->gameOver()) {
                numLives = 3;
                changeState(GAMEOVER);
            }
            else {
                if (level->getLevelCompleted()) {
					numLives = level->getLives();
                    currentLevel++;
                    if (currentLevel == 6) changeState(FINAL_COMIC);
                    else changeState(LOADING, currentLevel);
                }
            }
        }
        else if (type == SceneType::GAMEOVER) {
            GameOver* go = static_cast<GameOver*>(currentScene);
            if (go->hasFinished()) changeState(MAIN_MENU);
		}
    }

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentScene != nullptr) {
        currentScene->render();
    }

    // Mensajes de tutorial
    glUseProgram(0);
    actionMsg.render(uiProjection);
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
        return;
	}
    if (key == GLFW_KEY_B) {
        SceneType type = currentScene->getType();
        if (type != SceneType::CREDITS && type != SceneType::GAMEOVER) {
            changeState(MAIN_MENU, currentLevel);
            return;
        }
    }

    // Variar volumen música
    if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_P) {
        SoundManager::instance().increaseVolume(0.02f);
    }
    else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_O) {
        SoundManager::instance().decreaseVolume(0.02f);
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
    SoundManager::instance().setMusicSpeed(1.0f);

    if (currentScene != nullptr) {
        glFinish();
        delete currentScene; // Liberamos la memoria de la escena anterior
        currentScene = nullptr;
	}

    // Instanciamos la nueva escena seg�n el estado
    switch (newState) {
        case MAIN_MENU:
            currentScene = new MainMenu();
            SoundManager::instance().setMusicVolume(0.1f);
            SoundManager::instance().playMusic("menu", true);
            break;
        case INTIAL_COMIC:
			SoundManager::instance().stopMusic();
            currentScene = new Comic(0);
            break;
        case FINAL_COMIC:
            SoundManager::instance().setMusicVolume(0.02f); // Cinemática
            currentScene = new Comic(1);
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
            SoundManager::instance().setMusicVolume(0.15f);
            currentScene = new Credits();
            break;
        case LOADING:
            SoundManager::instance().stopMusic();
            currentScene = new LoadingScene(levelNumber);
		    break;
        case GAMEOVER:
            currentScene = new GameOver();
			SoundManager::instance().stopMusic();
            SoundManager::instance().playSound("gameOver", 0.75f);
			break;
    }

    // Inicializamos la nueva escena
    currentScene->init();
}

void Game::showTutorial(const string& msg)
{
    if (actionMsg.isActive()) return;
    if (messagesSeen.find(msg) == messagesSeen.end()) {
        actionMsg.show(msg);
        messagesSeen.insert(msg);
    }
}


