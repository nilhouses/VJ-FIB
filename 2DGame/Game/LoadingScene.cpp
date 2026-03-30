#include "LoadingScene.h"
#include "Game.h"
#include <iostream>


LoadingScene::LoadingScene(int nextLevel) : Scene(SceneType::LOADING) {
    this->nextLevel = nextLevel;
}


LoadingScene::~LoadingScene() {
	if (texProgram.isLinked()) {
        texProgram.free();
    }
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

    SoundManager::instance().playSound("elevator", 0.2f);
}


void LoadingScene::setupTips() {
	// Agregar tips reales del juego
    tips.push_back("TIP: COLLECT ALL SPACESHIP PARTS TO UNLOCK THE FINAL DOOR");
    tips.push_back("TIP: PUSH BARRELS TO CRUSH ENEMIES IN YOUR PATH");
    tips.push_back("TIP: PRESS [SPACE] TO FIRE YOUR GUN AT DANGEROUS FOES");
    tips.push_back("TIP: WATCH THE GEOMETRIC SIGNS, THEY LINK TUNNEL ENTRANCES");
    tips.push_back("TIP: PIPES CAN TRANSPORT YOU ACROSS THE ENTIRE LEVEL");
    tips.push_back("TIP: ACID IS LETHAL, ONE TOUCH AND IT'S GAME OVER!");
    tips.push_back("TIP: THE SPEEDBOOST IS FAST BUT MUCH HARDER TO CONTROL");
    tips.push_back("TIP: SOME ENEMIES PATROL, OTHERS HUNT, AND SOME SHOOT BACK");
    tips.push_back("TIP: SEARCH EVERY DOOR, SOME LEAD TO SECRET ROOMS");
    tips.push_back("TIP: USE LADDERS TO FLANK ENEMIES AND AVOID THEIR FIRE");
    tips.push_back("TIP: BEWARE! THE FIRE LEFT BY EXPLODING BARRELS DEALS DAMAGE");

    // Mezclar tips
    currentTipIndex = rand() % tips.size();
}


void LoadingScene::update(int deltaTime) {
    currentTime += deltaTime;
    totalTimeElapsed += deltaTime;

    // Animación de los puntos
    dotTimer += deltaTime;
    if (dotTimer > 500.f) {
        dotCount = (dotCount + 1) % 4; // Cicla entre 0, 1, 2, 3
        dotTimer = 0;
    }

    // Rotación de Pro-tips
    tipTimer += deltaTime;
    if (tipTimer > 2000.f || (Game::instance().getKey(GLFW_KEY_ENTER) && enterReleased)) {
        currentTipIndex = (currentTipIndex + 1) % tips.size();
        tipTimer = 0;
    }

    // Si ha pasado el tiempo suficiente, cambiamos al nivel real
    if (totalTimeElapsed >= loadingDuration) {
        Game::instance().changeState(PLAYING, nextLevel);
        return;
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
    // Usamos la función de texto que maneja sus propios shaders
    glUseProgram(0);
    // Render "LOADING..." centrado
    
    text.render(loadingStr, glm::vec2(250, 300), 20, glm::vec4(0.8f, 0.8f, 0.8f, 1), projection);
	
    int fontSize = 22;
    string levelStr;

    switch (nextLevel) {
	    case 1: levelStr = "LEVEL 1 - SANDY START"; break;
        case 2: levelStr = "LEVEL 2 - DUSTY GATES"; break;
        case 3: levelStr = "LEVEL 3 - UNDERGROUND CAVE"; break;
        case 4: levelStr = "LEVEL 4 - PIPE MAZE"; break;
        case 5: levelStr = "LEVEL 5 - GASTRIC GROOVE"; break;
    }
    float realWidth = levelStr.length() * (float)fontSize;
    float posX = (float(SCREEN_WIDTH) - realWidth) * 0.5f;
    text.render(levelStr, glm::vec2(posX, 220), fontSize, glm::vec4(1, 1, 1, 1), projection);

    // Render del Pro-tip
    // Centrado aproximado, restamos longitud del texto por tamaño de fuente/2
	int size = 12;
	if (tips[currentTipIndex].size() >= 58) size = 10;
    else if (tips[currentTipIndex].size() >= 53) size = 11;

    float tipPosX = 320.f - (tips[currentTipIndex].length() * size / 2.f);
    text.render(tips[currentTipIndex], glm::vec2(tipPosX, 400), size, glm::vec4(0.8f, 0.8f, 0.3f, 1), projection);
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