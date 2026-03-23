#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "Text.h"

class Credits : public Scene
{
public:
    Credits();
    ~Credits();

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

#endif // _CREDITS_INCLUDE