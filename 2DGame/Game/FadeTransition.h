#ifndef _FADE_TRANSITION_INCLUDE
#define _FADE_TRANSITION_INCLUDE

#include "Transition.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

class FadeTransition : public Transition {
public:
    // durationMs: tiempo en milisegundos
    // type: FADE_IN (negro -> invisible) o FADE_OUT (invisible -> negro)
    FadeTransition(FadeType type, float durationMs);
    virtual ~FadeTransition();

    void update(int deltaTime) override;
    void render() override;
    bool isFinished() const override { return finished; }

private:
    void initShader();
    void initQuad();

private:
    FadeType type;
    float currentTime = 0;
    float duration;
    float alpha = 0;
    bool finished = false;

    ShaderProgram shaderProgram;
    unsigned int vao, vbo;
    glm::mat4 projection;
};

#endif
