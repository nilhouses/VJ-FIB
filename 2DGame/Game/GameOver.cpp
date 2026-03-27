#include "GameOver.h"
#include "Game.h"
#include <iostream>

GameOver::GameOver() : Scene(SceneType::GAMEOVER) {
    background = nullptr;
    playerDead = nullptr;
}

GameOver::~GameOver() {
    if (background != nullptr) delete background;
    if (playerDead != nullptr) delete playerDead;
}

void GameOver::init() {
    initShaders();

    // 1. Càrrega de Textures
    texBackground.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texPlayerDead.loadFromFile("images/playersheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // 2. Crear Quads
    // Fons a pantalla completa
    glm::vec2 geomBG[2] = { {0.f, 0.f}, {float(SCREEN_WIDTH), float(SCREEN_HEIGHT)} };
    glm::vec2 texCoords[2] = { {0.f, 0.f}, {1.f, 1.f} };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);

    // Player
    float pw = 128.f; float ph = 128.f;
    float px = (SCREEN_WIDTH - pw) / 2.f;
    float py = (SCREEN_HEIGHT - ph) / 2.f - 20.f;
    glm::vec2 geomP[2] = { {0.f, 0.f}, {pw, ph} };
    playerDead = TexturedQuad::createTexturedQuad(geomP, texCoords, texProgram);

    if (!text.init("fonts/PressStart2P.ttf"))
        std::cout << "Could not load pixel font!!!" << std::endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void GameOver::update(int deltaTime) {
    timer += deltaTime;

    // Lògica del retard per mostrar el "PRESS B"
    if (timer > delayBeforePressB) {
        showPressB = true;
        fadeTimer += deltaTime;
        fadeAlpha = std::min(1.f, fadeTimer / 1000.f);
    }

    // Tornar al menú
    if (showPressB && (Game::instance().getKey(GLFW_KEY_B) || Game::instance().getKey(GLFW_KEY_ENTER))) {
        Game::instance().changeState(MAIN_MENU);
    }
}

void GameOver::render() {
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);

    // 1. Fondo
    modelview = glm::mat4(1.f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    background->render(texBackground);

    // 2. Foto Player
    modelview = glm::translate(glm::mat4(1.f), glm::vec3((SCREEN_WIDTH - 128.f) / 2.f, (SCREEN_HEIGHT - 128.f) / 2.f + 5, 0.f));
    texProgram.setUniformMatrix4f("modelview", modelview);
    playerDead->render(texPlayerDead);

    // 3. Textos
    glUseProgram(0);
    text.render("GAME OVER", glm::vec2(160, 100), 40, glm::vec4(1.f, 0.f, 0.f, 1.f), projection);

    if (showPressB) {
        float animY = float(SCREEN_HEIGHT) - 80.f + 5.f * sin(timer / 300.f);
        text.render("PRESS 'B' TO RETURN TO THE MENU", glm::vec2(65, animY), 16, glm::vec4(1.f, 1.f, 1.f, fadeAlpha), projection);
    }
}

void GameOver::initShaders() {
    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    texProgram.bindFragmentOutput("outColor");
}