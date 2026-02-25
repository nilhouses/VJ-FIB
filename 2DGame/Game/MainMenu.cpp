#include "MainMenu.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

MainMenu::MainMenu() {
    background = NULL;
    cursor = NULL;
}

MainMenu::~MainMenu() {
    if (background != NULL) delete background;
    if (cursor != NULL) delete cursor;
}

void MainMenu::init() {
    currentTime = 0.0f;
    selectedOption = 0;
    initShaders();

    // 1. Cargar Texturas
    texBackground.loadFromFile("images/menu_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texSprite.loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // 2. Crear Quads
    glm::vec2 geomBG[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);

    glm::vec2 geomCursor[2] = { glm::vec2(0.f, 0.f), glm::vec2(32.f, 32.f) };
    cursor = TexturedQuad::createTexturedQuad(geomCursor, texCoords, texProgram);

    // 3. Cargar Fuente
    if (!text.init("fonts/PressStart2P.ttf"))
        cout << "Could not load pixel font!!!" << std::endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}

void MainMenu::update(int deltaTime) {
    currentTime += deltaTime;
    timeSinceLastKey += deltaTime;

	if (timeSinceLastKey > 200) { // Para que la flecha no se salte opciones al pulsar una tecla
        if (Game::instance().getKey(GLFW_KEY_UP)) {
            selectedOption = (selectedOption - 1 + 3) % 3;
            timeSinceLastKey = 0;
        }
        else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
            selectedOption = (selectedOption + 1) % 3;
            timeSinceLastKey = 0;
        }

        if (Game::instance().getKey(GLFW_KEY_ENTER)) {
            if (selectedOption == 0) Game::instance().changeState(PLAYING, 1);
            else if (selectedOption == 1) Game::instance().changeState(INSTRUCTIONS);
            else if (selectedOption == 2) Game::instance().changeState(CREDITS);
            timeSinceLastKey = 0;
        }
    }
}

void MainMenu::render() {
    glm::mat4 modelview;

    // 1. Fondo y Cursor
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    // Fondo
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    background->render(texBackground);

    // Flecha
    float arrowY = 165.f + (selectedOption * 45.f);
    modelview = glm::translate(glm::mat4(1.0f), glm::vec3(205.f + 5*sin(currentTime / 200.f), arrowY, 0.f));
    texProgram.setUniformMatrix4f("modelview", modelview);
    cursor->render(texSprite);

    // 2. Textos
    glUseProgram(0);
    text.render("START", glm::vec2(250, 200), 32, glm::vec4(0, 0, 0, 1), projection);
    text.render("INSTRUCTIONS", glm::vec2(250, 245), 32, glm::vec4(0, 0, 0, 1), projection);
    text.render("CREDITS", glm::vec2(250, 290), 32, glm::vec4(0, 0, 0, 1), projection);
}

void MainMenu::initShaders() {
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}