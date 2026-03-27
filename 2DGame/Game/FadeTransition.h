#ifndef _FADE_TRANSITION_INCLUDE
#define _FADE_TRANSITION_INCLUDE

#include "Transition.h"
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>

class FadeTransition : public Transition {
public:
    // durationMs: tiempo en milisegundos
    // type: FADE_IN (negro -> invisible) o FADE_OUT (invisible -> negro)
    FadeTransition(bool toDark, float durationMs);
    virtual ~FadeTransition();

    void update(int deltaTime) override;
    void render() override;
    bool isFinished() const override { return finished; }

private:
    void initShaders();

private:
    bool toDark;
    float currentTime = 0;
    float duration;
    float alpha = 0;
    bool finished = false;

    TexturedQuad* background;
    ShaderProgram texProgram;
    glm::mat4 projection;
};

#endif
