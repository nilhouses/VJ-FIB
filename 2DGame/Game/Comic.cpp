#include "Comic.h"
#include "Game.h"
#include "SoundManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Comic::Comic(int version)
    : Scene(SceneType::COMIC)
    , version(version)
{
}

Comic::~Comic() {
    if (strip != nullptr) delete strip;
}

void Comic::init() {
    initShaders();

    if (version == 0) initIntro();
    else initOutro();

    float panelH = 1.f / float(numRows);
    float yTop = float(currentFrame) * panelH; 

    glm::vec2 geom[2] = { {0.f, 0.f}, {float(SCREEN_WIDTH), float(SCREEN_HEIGHT)} };
    glm::vec2 texCoords[2] = { {0.f, yTop}, {1.f, yTop + panelH} };

    strip = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

    if (!text.init("fonts/PressStart2P.ttf"))
        std::cout << "Could not load pixel font!!!" << std::endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void Comic::initIntro() {
    currentFrame = 1;
    SoundManager::instance().playSound("nave", 0.2f);
    texStrip.loadFromFile("images/comic_inicio.png", TEXTURE_PIXEL_FORMAT_RGBA);
    numRows = 10;
}

void Comic::initOutro() {
    SoundManager::instance().playSound("tuerca", 0.2f);
    texStrip.loadFromFile("images/comic_final.png", TEXTURE_PIXEL_FORMAT_RGBA);
    numRows = 12;
}

void Comic::update(int deltaTime) {
    if (finished) return;

    timer += deltaTime;
    autoTimer += deltaTime;

    if (onLastFrame) {
        fadeTimer += deltaTime;
        fadeAlpha = min(1.f, fadeTimer / 1000.f);

        if (Game::instance().getKey(GLFW_KEY_SPACE) && !autoSkip) {
            finished = true;
        }
        if (!Game::instance().getKey(GLFW_KEY_SPACE)) autoSkip = false;
        return;
    }

    // Avanzar por timer
    if (autoTimer >= AUTO_ADVANCE_MS) {
        autoTimer = 0.f;
        advanceFrame();
        return;
    }

    // Avanzar a mano
    if (Game::instance().getKey(GLFW_KEY_SPACE) && !autoSkip) {
        autoSkip = true;
        autoTimer = 0.f;
        advanceFrame();
    }
    else if (!Game::instance().getKey(GLFW_KEY_SPACE)) autoSkip = false;
}

void Comic::soundInitialComic() {
    switch (currentFrame) {
        case 2:
            SoundManager::instance().playSoundForce("turnPage2", 0.05f);
            break;
        case 3:
            SoundManager::instance().playSound("crash", 0.3f);
            SoundManager::instance().playSound("distracted", 2.f);
            break;
        case 4:
            SoundManager::instance().playSoundForce("alarm", 0.75f);
            break;
        case 6:
            SoundManager::instance().playSoundForce("spaceshipFalling", 0.6f);
            break;
        case 9:
            SoundManager::instance().playSoundForce("jump", 0.5f);
            SoundManager::instance().playSound("parachute", 1.f);
            break;
        default:
            break;
    }
}
void Comic::soundFinalComic() {
    switch (currentFrame) {
        case 1:
            SoundManager::instance().playSound("tuerca", 0.1f); 
            break;
        case 2:
            SoundManager::instance().playSoundForce("soplete", 0.3f);
            break;
        case 3:
            SoundManager::instance().playSoundForce("turnPage2", 0.05f);
            break;
        case 4:
            SoundManager::instance().playSoundForce("golpeMetal", 0.1f); 
            break;
        case 5:
            SoundManager::instance().playSoundForce("turnPage2", 0.05f);
            break;
        case 6:
            SoundManager::instance().playSoundForce("button", 0.3f);
            break;
        case 7:
            SoundManager::instance().playSoundForce("motorEncendiendose", 0.2f); 
            break;
        case 9:
            SoundManager::instance().playSoundForce("nosFuimos", 0.2f);
            SoundManager::instance().playSound("desgarroGusano", 0.4f);
            break;
        case 11:
            SoundManager::instance().playSound("shootingStar", 0.2f);
            break;
        case 12:
            break;
        default:
            break;
    }
}
void Comic::advanceFrame() {
    currentFrame++;

    if (currentFrame >= numRows) {
        currentFrame = numRows - 1;
        onLastFrame = true;
        fadeTimer = 0.f;
        fadeAlpha = 0.f;
        return;
    }

    if (version == 0) soundInitialComic();
    else soundFinalComic();
    float panelH = 1.f / float(numRows);
    float yTop = float(currentFrame) * panelH;

    glm::vec2 geom[2] = { {0.f, 0.f}, {float(SCREEN_WIDTH), float(SCREEN_HEIGHT)} };
    glm::vec2 texCoords[2] = { {0.f, yTop}, {1.f, yTop + panelH} };

    delete strip;
    strip = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void Comic::render() {
    glm::mat4 modelview = glm::mat4(1.f);

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);
    texProgram.setUniformMatrix4f("modelview", modelview);

    strip->render(texStrip);

    if (onLastFrame) {
        float animY = float(SCREEN_HEIGHT) - 40.f + 5.f * sin(timer / 300.f);
        text.render("PRESS [SPACE] TO CONTINUE", glm::vec2(140.f, animY), 14, glm::vec4(1.f, 1.f, 1.f, fadeAlpha), projection);
    }
}

void Comic::initShaders() {
    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    texProgram.bindFragmentOutput("outColor");
}