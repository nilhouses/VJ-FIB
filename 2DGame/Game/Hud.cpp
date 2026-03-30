#include "Hud.h"
#include "SpeedBoost.h"

Hud::Hud()
{

}

Hud::~Hud()
{
	if (background != nullptr) delete background;
}

void Hud::init(int numLives, int numKeys, int level)
{
    this->numLives = numLives;
    this->numKeys = numKeys;

    if (!text.init("fonts/PressStart2P.ttf"))
        cout << "Could not load font!" << endl;

    hudProj = glm::ortho(0.f, float(HUD_WIDTH), float(HUD_HEIGHT), 0.f);

    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/hudicon.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/hudicon.frag");
    hudProgram.init();
    hudProgram.addShader(vShader);
    hudProgram.addShader(fShader);
    hudProgram.link();
    hudProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();

	lifeIcon = new HudIcon();
	gunIcon = new HudIcon();
	boostIcon = new HudIcon();
	keyIcon = new HudIcon();

    lifeIcon->init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.50f, 0.f));
    gunIcon->init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.25f, 1.f/5.f));
    boostIcon->init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.25f, 0.f));
    keyIcon->init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.f, 0.f));

	initShaders();

    // Fondo del HUD
    string hudBgPath;
    if (level == 5) hudBgPath = "images/hud_lvl5.png"; // Fondo especial para el nivel final
	else hudBgPath = "images/hud.png";
	texBackground.loadFromFile(hudBgPath, TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 geomBG[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(HUD_WIDTH), float(HUD_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);
}

void Hud::update(int deltaTime, int numLives, int numBullets, float speedBoostDuration, float boostTimeLeft, int collectedKeys, bool godMode)
{
	this->numLives = numLives;
	this->numKeys = collectedKeys;
	this->numBullets = numBullets;
	this->speedBoostDuration = speedBoostDuration;
	this->boostTimeLeft = boostTimeLeft;
	this->godMode = godMode;
}

void Hud::render()
{
    // Fondo del HUD
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", hudProj);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
    background->render(texBackground);

    glUseProgram(0);

    // Preparamos strings
    std::string livesStr = "x" + std::to_string(numLives);
    std::string gunStr = "x" + std::to_string(numBullets);
    std::string keysStr = std::to_string(numKeys) + "/" + std::to_string(allKeys);

    float iconsY = 16.f;
    float margin = 25.f;  

    // Texto +  iconos agrupados a la derecha

    float textY = 45.f;
    float step = 100.f;

    // Vidas
    float livesX = margin;
    lifeIcon->render(glm::vec2(livesX, iconsY), hudProj, 1.f);
    text.render(livesStr, glm::vec2(livesX + 45.f, textY), 24, glm::vec4(1, 1, 1, 1), hudProj);

    // God mode
    float godModeX = livesX + 110.f;
    if (godMode)
        text.render("GOD MODE", glm::vec2(godModeX, textY), 24, glm::vec4(1, 0.5f, 0.5f, 1), hudProj);

    // Llaves
    float keysX = HUD_WIDTH - margin - 70.f - 40.f;
    keyIcon->render(glm::vec2(keysX, iconsY), hudProj, 1.f);
    glm::vec4 keyCol = (numKeys == allKeys) ? glm::vec4(1, 1, 0.2f, 1) : glm::vec4(1, 1, 1, 1);
    text.render(keysStr, glm::vec2(keysX + 40.f, textY), 24, keyCol, hudProj);

    // Pistola
    float gunX = keysX - 110.f;
    gunIcon->render(glm::vec2(gunX, iconsY), hudProj, 1.f);
    text.render(gunStr, glm::vec2(gunX + 40.f, textY), 24, glm::vec4(1, 1, 1, 1), hudProj);

    // Speedboost
    float boostX = gunX - 50.f;
    float fill = (boostTimeLeft > 0.0f) ? (boostTimeLeft / SpeedBoost::getMaxTime()) : 0.f;
    boostIcon->render(glm::vec2(boostX, iconsY), hudProj, fill);
}


void Hud::initShaders() {
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