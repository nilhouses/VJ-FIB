#ifndef _COMIC_INCLUDE
#define _COMIC_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Text.h"
#include <glm/glm.hpp>

class Comic : public Scene {
public:
    Comic(int version);  // 0 = intro, 1 = outro
    ~Comic();

    void init()                override;
    void update(int deltaTime) override;
    void render()              override;

    bool isInitial() { return version == 0; }
    bool hasFinished() { return finished; }
	void soundInitialComic();
	void soundFinalComic();

private:
    void initShaders();
    void advanceFrame();
    void initIntro();
    void initOutro();

    int version = -1;

    TexturedQuad* strip = nullptr;
    Texture       texStrip = Texture();
    ShaderProgram texProgram = ShaderProgram();
    Text          text = Text();
    glm::mat4     projection = glm::mat4(1.f);

    int currentFrame = 0;
    int numRows = 0;
    bool finished = false;
    bool onLastFrame = false;


    float timer = 0.f;
    float autoTimer = 0.f;
    float fadeTimer = 0.f;
    float fadeAlpha = 0.f;
	bool autoSkip = false;

    static const int AUTO_ADVANCE_MS = 2000;
};

#endif // _COMIC_INCLUDE