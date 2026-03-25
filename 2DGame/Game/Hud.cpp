#include "Hud.h"
#include "SpeedBoost.h"

Hud::Hud()
{

}

Hud::~Hud()
{

}

void Hud::init(int numLives, int numKeys)
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

}

void Hud::update(int deltaTime, int numLives, int numBullets, float speedBoostDuration, float boostTimeLeft, int collectedKeys)
{
	this->numLives = numLives;
	this->numKeys = collectedKeys;
	this->numBullets = numBullets;
	this->speedBoostDuration = speedBoostDuration;
	this->boostTimeLeft = boostTimeLeft;
}

void Hud::render()
{
    // Preparamos strings
    std::string livesStr = "x" + std::to_string(numLives);
    std::string gunStr   = "x" + std::to_string(numBullets);
    std::string keysStr  = std::to_string(numKeys) + "/" + std::to_string(allKeys);

    glUseProgram(0);


    float iconsY = 16.f;
    float textY = 45.f;

	// Texto +  iconos agrupados a la derecha
    
    // Llaves
    float keysGroupX = HUD_WIDTH - 120.f;
    keyIcon.render(glm::vec2(keysGroupX, iconsY), hudProj, 0.f);
    text.render(keysStr, glm::vec2(keysGroupX + 40.f, textY), 24, glm::vec4(1, 1, 0.2f, 1), hudProj);

    // Speedboost   
    float boostGroupX = keysGroupX - 80.f;
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
}