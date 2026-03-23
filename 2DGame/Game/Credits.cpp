#include "Credits.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Credits::Credits() : Scene(SceneType::CREDITS) {
    background = NULL;
}

Credits::~Credits() {
    if (background != NULL) delete background;
}

void Credits::init() {
    initShaders();

    texBackground.loadFromFile("images/menu_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);

    glm::vec2 geomBG[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);

    if (!text.init("fonts/PressStart2P.ttf"))
        cout << "Could not load pixel font!!!" << endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void Credits::update(int deltaTime) {
	timer += deltaTime;

    if (Game::instance().getKey(GLFW_KEY_ESCAPE) || Game::instance().getKey(GLFW_KEY_B)) {
        Game::instance().changeState(MAIN_MENU);
    }
}

void Credits::render() {
    glm::mat4 modelview;

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    // Fondo
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    background->render(texBackground);

    // Texto
    glm::vec4 yellow = glm::vec4(1, 1, 0, 1);
    glm::vec4 lightBlue = glm::vec4(0.4f, 0.8f, 1.0f, 1);
    glm::vec4 black = glm::vec4(0, 0, 0, 1);
    glm::vec4 red = glm::vec4(1.0f, 0.4f, 0.4f, 1);

    text.render("CREDITS", glm::vec2(185, 80), 40, yellow, projection); // Verde claro

    text.render("DEVELOPERS:", glm::vec2(100, 180), 20, black, projection);
    text.render("Alejandro Ruiz Patn", glm::vec2(100, 230), 25, black, projection);
    text.render("Nil Casas Duatis", glm::vec2(100, 280), 25, black, projection);
    
    text.render("ART DESIGN:", glm::vec2(100, 350), 20, black, projection);
    text.render("Adragong3", glm::vec2(100, 390), 20, black, projection);

    float animY = 440.0f + 5.0f * sin(timer / 300.f);
    text.render("PRESS 'B' TO GO BACK", glm::vec2(165, animY), 16, black, projection);
}

void Credits::initShaders() {
    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    texProgram.bindFragmentOutput("outColor");
}