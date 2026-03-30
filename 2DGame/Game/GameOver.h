#ifndef _GAMEOVER_INCLUDE
#define _GAMEOVER_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Text.h"

class GameOver : public Scene {
public:
    GameOver();
    ~GameOver();
    void init() override;
    void update(int deltaTime) override;
    void render() override;
	bool hasFinished() { return B_Pressed; } 

private:
    void initShaders();

    TexturedQuad* background = nullptr;
    TexturedQuad* playerDeadQuad = nullptr;
    Texture texBackground = Texture();
    Texture texPlayerDead = Texture();

    ShaderProgram texProgram = ShaderProgram();
    Text text = Text();

    float timer = 0.f;
	float delayBeforePressB = 4500.f; // 1.5 segundos de espera antes de mostrar al personaje muerto y el mensaje de "PRESS B"
    float fadeTimer = 0.f;
    float fadeAlpha = 0.f;
    bool showPressB = false;
	bool B_Pressed = false;

};

#endif