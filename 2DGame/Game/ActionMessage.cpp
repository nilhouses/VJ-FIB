#include "ActionMessage.h"
#include "Game.h"

void ActionMessage::init(Text* textPtr) {
    text = textPtr;
    active = false;
    alpha = 0.0f;
}

void ActionMessage::show(const std::string& msg, float durationMs) {
    currentMsg = msg;
    timer = durationMs;
    active = true;
}

void ActionMessage::update(int deltaTime) {
    if (!active) return;

    timer -= deltaTime;
    if (timer > 0) {
        alpha = glm::min(1.0f, alpha + deltaTime / 500.f); // Fade in
    }
    else {
        alpha = glm::max(0.0f, alpha - deltaTime / 500.f); // Fade out
        if (alpha <= 0.0f) active = false;
    }
}

void ActionMessage::render(const glm::mat4& projection) {
    if (!active) return;

    int fontSize = 10;
    float realWidth = currentMsg.length() * (float)fontSize;

    float posX = (float(SCREEN_WIDTH) - realWidth) * 0.5f;
    if (posX < 10.f) posX = 10.f;
    float posY = float(LEVEL_HEIGHT) - 34.0f;

    text->render(currentMsg, glm::vec2(posX, posY), fontSize, glm::vec4(1, 1, 1, alpha), projection);
}