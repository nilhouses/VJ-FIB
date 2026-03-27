#include "FadeTransition.h"
#include "Game.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


FadeTransition::FadeTransition(bool toDark, float durationMs) {
    this->toDark = toDark;
    this->currentTime = 0.f;
    this->duration = durationMs;
    this->finished = false;
    this->alpha = (toDark) ? 0.f : 1.f;
    initShaders();

    // Crear Quad
    glm::vec2 geomBG[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT)) };
    glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
}


FadeTransition::~FadeTransition() {
	if (background != NULL) delete background;
}


void FadeTransition::update(int deltaTime) {
    if (finished) return;

    currentTime += deltaTime;
    float progress = currentTime / duration;

    if (progress >= 1.0f) {
        progress = 1.0f;
        finished = true;
    }

    if (toDark) alpha = progress;
    else alpha = 1.f - progress;
}


void FadeTransition::render() {
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, alpha); // Negro con el alpha calculado

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    background->render();

    glUseProgram(0);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

void FadeTransition::initShaders() {
    // Shaders inline por simplicidad
    string vShaderStr =
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        "uniform mat4 projection;\n"
        "void main() { gl_Position = projection * vec4(position, 0.0, 1.0); }";

    string fShaderStr =
        "#version 330 core\n"
        "out vec4 outColor;\n"
        "uniform vec4 color;\n"
        "void main() { outColor = color; }";

    Shader vShader, fShader;
    vShader.initFromSource(VERTEX_SHADER, vShaderStr);
    fShader.initFromSource(FRAGMENT_SHADER, fShaderStr);

    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
}