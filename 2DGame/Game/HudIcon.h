#ifndef _HUDICON_INCLUDE
#define _HUDICON_INCLUDE

#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture.h"

class HudIcon {

public:
    HudIcon() {}
    ~HudIcon() { if (sprite) delete sprite; }

    void init(ShaderProgram& program, const string& sheet, glm::ivec2 size, glm::vec2 sizeInSheet, glm::vec2 frame);
    void render(glm::vec2 pos, const glm::mat4& proj, float fill = 0.f);

private:
    Sprite* sprite = nullptr;
    Texture tex;
    ShaderProgram* prog = nullptr;
};

#endif // _HUDICON_INCLUDE
