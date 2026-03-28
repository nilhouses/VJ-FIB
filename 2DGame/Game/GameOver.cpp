#include "GameOver.h"
#include "Game.h"
#include <iostream>

GameOver::GameOver() : Scene(SceneType::GAMEOVER) {
    background = nullptr;
    playerDeadQuad = nullptr;
}


GameOver::~GameOver() {
    if (background != nullptr) delete background;
    if (playerDeadQuad != nullptr) delete playerDeadQuad;
}

void GameOver::init() {
    initShaders();

    // Texturas
    texBackground.loadFromFile("images/death_screen.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texPlayerDead.loadFromFile("images/sprite_sheet_animations.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // Background
    glm::vec2 geomBG[2] = { {0.f, 0.f}, {float(SCREEN_WIDTH), float(SCREEN_HEIGHT)} };
    glm::vec2 texCoordsBG[2] = { {0.f, 0.f}, {1.f, 1.f} };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoordsBG, texProgram);

    // Player
    float frameW = 1.f / 16.f;
    float frameH = 1.f / 12.f;
    float startX = 3.f * frameW;
    float startY = 3.f * frameH;

    glm::vec2 geomP[2] = { {0.f, 0.f}, {128.f, 128.f} };
    glm::vec2 texCoordsP[2] = {
        {startX, startY},                   // Top Left
        {startX + frameW, startY + frameH}  // Bottom Right
    };

    playerDeadQuad = TexturedQuad::createTexturedQuad(geomP, texCoordsP, texProgram);
    playerDeadQuad = TexturedQuad::createTexturedQuad(geomP, texCoordsP, texProgram);

    if (!text.init("fonts/PressStart2P.ttf"))
        std::cout << "Could not load pixel font!!!" << std::endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void GameOver::update(int deltaTime) {
    timer += deltaTime;

    if (timer > delayBeforePressB) {
        showPressB = true;
        fadeTimer += deltaTime;
        fadeAlpha = std::min(1.f, fadeTimer / 1000.f);
    }
    if (showPressB && Game::instance().getKey(GLFW_KEY_B))
        B_Pressed = true;
}

void GameOver::render() {
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);

    // Fondo
    modelview = glm::mat4(1.f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    background->render(texBackground);

    // Player
    modelview = glm::translate(glm::mat4(1.f), glm::vec3((SCREEN_WIDTH - 128.f) / 2.f, (SCREEN_HEIGHT - 128.f) / 2.f - 5, 0.f));
    texProgram.setUniformMatrix4f("modelview", modelview);
    playerDeadQuad->render(texPlayerDead);

    // Texto
    glUseProgram(0);

    string msg1 = "GAME OVER";
    int size1 = 40;
    float width1 = msg1.length() * (float)size1;
    float posX1 = (float(SCREEN_WIDTH) - width1) * 0.5f;

    float redFade = std::min(1.f, timer / 3000.f);
    text.render(msg1, glm::vec2(posX1, 100), size1, glm::vec4(redFade, 0.f, 0.f, 1.f), projection);
    if (showPressB) {
        string msg2 = "PRESS 'B' TO RETURN TO THE MAIN MENU";
        int size2 = 16;
        float width2 = msg2.length() * (float)size2;
        float posX2 = (float(SCREEN_WIDTH) - width2) * 0.5f;

        float animY = float(SCREEN_HEIGHT) - 80.f + 5.f * sin(timer / 300.f);
        text.render(msg2, glm::vec2(posX2, animY), size2, glm::vec4(1.f, 1.f, 1.f, fadeAlpha), projection);
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