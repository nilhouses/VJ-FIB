#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Text.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct CreditEntry {
    std::string  label;
    int          fontSize;
    glm::vec4    color;
    float        marginTop;
};

class Credits : public Scene {
public:
    Credits();
    ~Credits();
    void init()            override;
    void update(int deltaTime) override;
    void render()          override;

private:
    void initShaders();
    void buildCredits();

    CreditEntry makeTitle(const std::string& label);
    CreditEntry makeSection(const std::string& label);
    CreditEntry makeName(const std::string& label);

    TexturedQuad* background = nullptr;
    Texture       texBackground = Texture();
    ShaderProgram texProgram = ShaderProgram();
    Text          text = Text();
    glm::mat4     projection = glm::mat4(1.f);

    // Scroll
	float startDelay = 500.f; // medio segundo de delay antes de empezar a scrollear
    float scrollOffset = 0.f;
    float scrollSpeed = 0.05f;  // px/ms
    bool  scrollDone = false;
    float totalContentHeight = 0.f;
    float timer = 0.f;

    // "PRESS B"
    float fadeTimer = 0.f;
    float fadeAlpha = 0.f;

    // Contenido
    std::vector<CreditEntry> text_entries = vector<CreditEntry>();
    std::vector<float>       text_entryY = vector<float>();
};

#endif // _CREDITS_INCLUDE



