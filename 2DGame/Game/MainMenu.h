#ifndef _MAINMENU_INCLUDE
#define _MAINMENU_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "Text.h"

class MainMenu : public Scene
{
public:
    MainMenu();
    ~MainMenu();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();

private:
    Texture texBackground = Texture(), texSprite = Texture();
    TexturedQuad* background = nullptr, * cursor = nullptr;
    Text text = Text();
    ShaderProgram texProgram = ShaderProgram();
    glm::mat4 projection = glm::mat4(1.f);

    int selectedOption = 0; // [0,1,2]
    int timeSinceLastKey = 0;
    float currentTime = 0.f;
};

#endif