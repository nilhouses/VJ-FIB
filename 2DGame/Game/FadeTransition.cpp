#include "FadeTransition.h"
#include "Game.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


FadeTransition::FadeTransition(FadeType type, float durationMs) {
    this->type = type;
    this->duration = durationMs;
    this->finished = false;

    // Si es FadeIn empezamos en negro total (1), si es FadeOut en transparente (0)
    this->alpha = (type == FADE_IN) ? 1.0f : 0.0f;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

    initShader();
    initQuad();
}


FadeTransition::~FadeTransition() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}


void FadeTransition::update(int deltaTime) {
    if (finished) return;

    currentTime += deltaTime;
    float progress = currentTime / duration;

    if (progress >= 1.0f) {
        progress = 1.0f;
        finished = true;
    }

    if (type == FADE_IN) {
        alpha = 1.0f - progress; // De 1 a 0
    }
    else {
        alpha = progress;        // De 0 a 1
    }
}


void FadeTransition::render() {
    shaderProgram.use();
    shaderProgram.setUniformMatrix4f("projection", projection);
    shaderProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, alpha); // Negro con el alpha calculado

    // Configurar Blending para que el alpha funcione
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST); // Dibujar encima de todo sin importar la profundidad

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}


void FadeTransition::initQuad() {
    // Un quad simple que cubra toda la pantalla usando coordenadas de píxeles
    float vertices[] = {
        0.0f, 0.0f,
        float(SCREEN_WIDTH), 0.0f,
        0.0f, float(SCREEN_HEIGHT),

        0.0f, float(SCREEN_HEIGHT),
        float(SCREEN_WIDTH), 0.0f,
        float(SCREEN_WIDTH), float(SCREEN_HEIGHT)
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}


void FadeTransition::initShader() {
    // Shaders inline por simplicidad
    string vShaderStr =
        "#version 330 core\n"
        "layout(location = 0) in vec2 pos;\n"
        "uniform mat4 projection;\n"
        "void main() { gl_Position = projection * vec4(pos, 0.0, 1.0); }";

    string fShaderStr =
        "#version 330 core\n"
        "out vec4 outColor;\n"
        "uniform vec4 color;\n"
        "void main() { outColor = color; }";

    Shader vShader, fShader;
    vShader.initFromSource(VERTEX_SHADER, vShaderStr);
    fShader.initFromSource(FRAGMENT_SHADER, fShaderStr);

    shaderProgram.init();
    shaderProgram.addShader(vShader);
    shaderProgram.addShader(fShader);
    shaderProgram.link();
}