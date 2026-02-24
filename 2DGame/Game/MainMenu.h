#ifndef _MAINMENU_INCLUDE
#define _MAINMENU_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "Text.h"

// Clase para la pantalla del menú principal
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
    Texture texBackground, texSprite;
    TexturedQuad* background, * cursor;
    Text text;
    ShaderProgram texProgram;
    glm::mat4 projection;

    int selectedOption = 0; // [0,1,2]
    int timeSinceLastKey = 0;
    float currentTime;
};

#endif