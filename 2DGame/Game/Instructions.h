#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "Text.h"

class Instructions : public Scene
{
public:
    Instructions();
    ~Instructions();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();

private:
    Texture texBackground = Texture();
    TexturedQuad* background = nullptr;
    Text text = Text();
    ShaderProgram texProgram = ShaderProgram();
    glm::mat4 projection = glm::mat4(1.f);

    int timer = 0;
};

#endif // _INSTRUCTIONS_INCLUDE