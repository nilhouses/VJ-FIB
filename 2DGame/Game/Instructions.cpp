#include "Instructions.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Instructions::Instructions() : Scene(SceneType::INSTRUCTIONS) {
    background = NULL;
}

Instructions::~Instructions() {
    if (background != NULL) delete background;
}

void Instructions::init() {
    initShaders();
    texBackground.loadFromFile("images/fondo_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);

    glm::vec2 geomBG[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);

    if (!text.init("fonts/PressStart2P.ttf"))
        cout << "Could not load pixel font!!!" << endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void Instructions::update(int deltaTime) {
	timer += deltaTime;

    // Si el jugador pulsa ESC o B, volvemos al men� principal
    if (Game::instance().getKey(GLFW_KEY_ESCAPE) || Game::instance().getKey(GLFW_KEY_B)) {
        Game::instance().changeState(MAIN_MENU);
    }
}

void Instructions::render() {
    glm::mat4 modelview;

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    // Dibujar Fondo
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    background->render(texBackground);

    // Dibujar Textos de Instrucciones
    // Colores claros para que se vean sobre fondo oscuro
    glm::vec4 yellow = glm::vec4(1, 1, 0, 1);
    glm::vec4 lightBlue = glm::vec4(0.289f, 0.504f, 9.922f, 1.f);
    glm::vec4 black = glm::vec4(0, 0, 0, 1);
    glm::vec4 red = glm::vec4(1.0f, 0.4f, 0.4f, 1);
	glm::vec4 white = glm::vec4(1, 1, 1, 1);

    // TÍTULO PRINCIPAL
    text.render("HOW TO PLAY", glm::vec2(150, 60), 32, yellow, projection);

    // CONTROLES
	int initY = 120;
    text.render("CONTROLS", glm::vec2(40, initY), 22, lightBlue, projection);
    text.render("LEFT/RIGHT: Move & Push Barrels", glm::vec2(60, initY + 30.f), 14, white, projection);
    text.render("UP/DOWN: Climb Stairs & Enter Pipes", glm::vec2(60, initY + 60.f), 14, white, projection);
    text.render("UP: Enter Doors & Enter Tunnels", glm::vec2(60, initY + 90.f), 14, white, projection);
    text.render("Q: Fire Weapon", glm::vec2(60, initY + 120.f), 14, white, projection);
	text.render("O/P: Change Music Volume", glm::vec2(60, initY + 150.f), 14, white, projection);

    // MISSION OBJECTIVE
	initY = 310;
    text.render("MISSION OBJECTIVE", glm::vec2(40, initY), 22, lightBlue, projection);
    text.render("Find all spaceship parts to", glm::vec2(60, initY + 30.f), 16, white, projection);
    text.render("reach the next level.", glm::vec2(58, initY + 60.f), 16, white, projection);

    // VOLVER
    float animY = 420.0f + 5.0f * sin(timer / 300.f);
    text.render("PRESS 'B' TO GO BACK", glm::vec2(165, animY), 16, white, projection);
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