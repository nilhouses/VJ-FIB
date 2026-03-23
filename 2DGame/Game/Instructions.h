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
    Texture texBackground;
    TexturedQuad* background;
    Text text;
    ShaderProgram texProgram;
    glm::mat4 projection;

    int timer = 0;
};

#endif // _INSTRUCTIONS_INCLUDE