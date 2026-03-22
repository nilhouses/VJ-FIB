#include "LoadingScene.h"
#include "Game.h"
#include <iostream>


LoadingScene::LoadingScene(int nextLevel) : Scene(SceneType::LOADING) {
    this->nextLevel = nextLevel;
}


LoadingScene::~LoadingScene() {
}


void LoadingScene::init() {
    currentTime = 0.0f;
    dotTimer = 0.0f;
    tipTimer = 0.0f;
    totalTimeElapsed = 0.0f;

    initShaders();
    setupTips();

    if (!text.init("fonts/PressStart2P.ttf"))
        std::cout << "Could not load font for Loading Scene!" << std::endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}


void LoadingScene::setupTips() {
	// TODO: Agregar tips reales del juego
    tips.push_back("TIP: USE UP ARROW TO ENTER DOORS");
    tips.push_back("TIP: COLLECT ALL KEYS TO UNLOCK THE EXIT");
    tips.push_back("TIP: BLOCKS CAN BE PUSHED TO ACTIVATE SWITCHES");
    tips.push_back("TIP: BEWARE OF THE SPIKES, THEY HURT!");

    // Mezclar tips
    currentTipIndex = rand() % tips.size();
}


void LoadingScene::update(int deltaTime) {
    currentTime += deltaTime;
    totalTimeElapsed += deltaTime;

    // Animación de los puntos (cada 500ms cambia)
    dotTimer += deltaTime;
    if (dotTimer > 500.f) {
        dotCount = (dotCount + 1) % 4; // Cicla entre 0, 1, 2, 3
        dotTimer = 0;
    }

    // Rotación de Pro-tips (cada 2 segundos cambia)
    tipTimer += deltaTime;
    if (tipTimer > 2000.f || Game::instance().getKey(GLFW_KEY_ENTER) && enterReleased) {
        currentTipIndex = (currentTipIndex + 1) % tips.size();
        tipTimer = 0;
    }

    // Si ha pasado el tiempo suficiente, cambiamos al nivel real
    if (totalTimeElapsed >= loadingDuration) {
        Game::instance().changeState(PLAYING, nextLevel);
    }

    enterReleased = !Game::instance().getKey(GLFW_KEY_ENTER);
}


void LoadingScene::render() {
    // Fondo
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Preparamos el string de "Loading" con los puntos
    std::string loadingStr = "LOADING";
    for (int i = 0; i < dotCount; ++i) loadingStr += ".";

    // Render de textos
    glUseProgram(0); // Usamos la función de texto que maneja sus propios shaders

    // Render "LOADING..." centrado
    text.render(loadingStr, glm::vec2(200, 220), 32, glm::vec4(1, 1, 1, 1), projection);

    // Render del Pro-tip
    // Centrado aproximado, restamos longitud del texto por tamaño de fuente/2
    float tipPosX = 320.f - (tips[currentTipIndex].length() * 12.f / 2.f);
    text.render(tips[currentTipIndex], glm::vec2(tipPosX, 400), 12, glm::vec4(0.8f, 0.8f, 0.3f, 1), projection);
}


void LoadingScene::initShaders() {
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