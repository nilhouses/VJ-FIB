#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GL/gl.h>
#include "Circle.h"
#include <vector>
#include <cmath>

Circle* Circle::createCircle(float x, float y, float radius, ShaderProgram& program)
{
    return new Circle(x, y, radius, program);
}

Circle::Circle(float x, float y, float radius, ShaderProgram& program)
{
    const int N = 30;
    vector<float> vertices;

    // Center
    vertices.push_back(x);
    vertices.push_back(y);

    // Perímeter
    for (int i = 0; i <= N; ++i) {
        float angle = 2.0f * float(M_PI) * float(i) / float(N);
        vertices.push_back(x + radius * cos(angle));
        vertices.push_back(y + radius * sin(angle));
    }

    numVertices = vertices.size() / 2;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    posLocation = program.bindVertexAttribute("position", 2);
}

void Circle::render() const
{
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    // TRIANGLE_FAN connects the first vertex with the rest
    glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
}

void Circle::free()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}