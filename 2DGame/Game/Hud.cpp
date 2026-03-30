#include "Hud.h"
#include "SpeedBoost.h"

Hud::Hud()
{

}

Hud::~Hud()
{

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

    lifeIcon.init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.50f, 0.f));
    gunIcon.init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.25f, 1.f/5.f));
    boostIcon.init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.25f, 0.f));
    keyIcon.init(hudProgram, "images/items.png", glm::ivec2(32, 32), glm::vec2(0.25f, 1.f / 5.f), glm::vec2(0.f, 0.f));

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

    // Preparamos strings
    std::string livesStr = "x" + std::to_string(numLives);
    std::string gunStr   = "x" + std::to_string(numBullets);
    std::string keysStr  = std::to_string(numKeys) + "/" + std::to_string(allKeys);

    float iconsY = 16.f;
    float textY = 45.f;

	// Texto +  iconos agrupados a la derecha
    
    // Llaves
    float keysGroupX = HUD_WIDTH - 130.f;
    keyIcon.render(glm::vec2(keysGroupX, iconsY), hudProj, 0.f);
    if (numKeys == allKeys) text.render(keysStr, glm::vec2(keysGroupX + 40.f, textY), 24, glm::vec4(1, 1, 0.2f, 1), hudProj);
    else text.render(keysStr, glm::vec2(keysGroupX + 40.f, textY), 24, glm::vec4(1, 1, 1, 1), hudProj);

    // Speedboost   
    float boostGroupX = keysGroupX - 60.f;
    if (boostTimeLeft > 0.0f) {
        float fill = glm::clamp(boostTimeLeft / SpeedBoost::getMaxTime(), 0.0f, 1.0f);
        boostIcon.render(glm::vec2(boostGroupX, iconsY), hudProj, fill);
    }
    else boostIcon.render(glm::vec2(boostGroupX, iconsY), hudProj, 0.f);

    // Pipa
    float gunGroupX = boostGroupX - 120.f;
    gunIcon.render(glm::vec2(gunGroupX, iconsY), hudProj, 0.f);
    text.render(gunStr, glm::vec2(gunGroupX + 40.f, textY), 24, glm::vec4(1, 1, 1, 1), hudProj);

    // Vidas
    float livesGroupX = gunGroupX - 120.f;
    lifeIcon.render(glm::vec2(livesGroupX, iconsY), hudProj, 0.f);
    text.render(livesStr, glm::vec2(livesGroupX + 40.f, textY), 24, glm::vec4(1, 1, 1, 1), hudProj);

    // GodMode (opcional)
    float godModeGroupX = livesGroupX - 200.f;
    if (godMode) text.render("GOD MODE", glm::vec2(godModeGroupX, textY), 24, glm::vec4(1, 0.5f, 0.5f, 1), hudProj);

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