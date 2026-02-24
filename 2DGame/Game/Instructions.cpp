#include "Instructions.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Instructions::Instructions() {
    background = NULL;
}

Instructions::~Instructions() {
    if (background != NULL) delete background;
}

void Instructions::init() {
    initShaders();
    texBackground.loadFromFile("images/menu_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);

    glm::vec2 geomBG[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);

    if (!text.init("fonts/PressStart2P.ttf"))
        std::cout << "Could not load pixel font!!!" << std::endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void Instructions::update(int deltaTime) {
    // Si el jugador pulsa ESC o B, volvemos al menú principal
    if (Game::instance().getKey(GLFW_KEY_ESCAPE) || Game::instance().getKey(GLFW_KEY_B)) {
        Game::instance().changeState(MAIN_MENU);
    }
}

void Instructions::render() {
    glm::mat4 modelview;

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    // 1. Dibujar Fondo
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    background->render(texBackground);

    // 2. Dibujar Textos de Instrucciones
    text.render("HOW TO PLAY", glm::vec2(100, 80), 40, glm::vec4(0, 0, 0, 1));

    text.render("ARROWS: Move player", glm::vec2(100, 180), 20, glm::vec4(0, 0, 0, 1));
    // [TODO]...
    //
    //
    //

    text.render("PRESS 'B' TO GO BACK", glm::vec2(150, 475), 18, glm::vec4(1.f, 1.f, 1.f, 1));
}

void Instructions::initShaders() {
    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    texProgram.bindFragmentOutput("outColor");
}