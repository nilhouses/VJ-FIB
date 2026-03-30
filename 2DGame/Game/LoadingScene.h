#ifndef _LOADING_SCENE_INCLUDE
#define _LOADING_SCENE_INCLUDE

#include "Scene.h"
#include "Text.h"
#include "ShaderProgram.h"
#include <vector>
#include <string>
#include "SoundManager.h"

class LoadingScene : public Scene
{
public:
    // Le pasamos el nivel al que queremos ir después de cargar
    LoadingScene(int nextLevel);
    ~LoadingScene();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();
    void setupTips();

private:
    Text text = Text();
    ShaderProgram texProgram = ShaderProgram();

    float currentTime = 0.f;
    int nextLevel = -1;

    // Lógica de los puntos (...)
    int dotCount = 0;
    float dotTimer = 0;

    // Lógica de los Tips
    vector<string> tips = vector<string>();
    int currentTipIndex = 0;
    float tipTimer = 0;

    // Tiempo total que queremos que dure la pantalla de carga (ms)
    float loadingDuration = 4000.0f;
    float totalTimeElapsed = 0;

    bool enterReleased = false;
};

#endif