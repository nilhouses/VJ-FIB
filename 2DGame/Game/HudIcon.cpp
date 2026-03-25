#include "HudIcon.h"

void HudIcon::init(ShaderProgram& program, const string& sheet, glm::ivec2 size, glm::vec2 sizeInSheet, glm::vec2 frame) {
    prog = &program;
    tex.loadFromFile(sheet, TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(size, sizeInSheet, &tex, &program, nullptr);
    sprite->setNumberAnimations(1);
    sprite->addKeyframe(0, frame);
    sprite->changeAnimation(0);
}

void HudIcon::render(glm::vec2 pos, const glm::mat4& proj, float fill) {
    sprite->setPosition(pos);
    prog->use();
    prog->setUniform1f("fillAmount", fill);
    sprite->render(proj);
}
