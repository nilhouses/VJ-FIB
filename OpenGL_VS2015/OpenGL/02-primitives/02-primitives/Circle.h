#pragma once
#ifndef _CIRCLE_INCLUDE
#define _CIRCLE_INCLUDE


#include "ShaderProgram.h"


// Given a point (x, y) and a radius in pixel Circle creates 
// a VBO with N triangles. Vertices in the VBO have only a position
// attribute (no colors or texture coordinates)


class Circle
{
private:
    Circle(float x, float y, float radius, ShaderProgram& program);

public:
    static Circle* createCircle(float x, float y, float radius, ShaderProgram& program);

    void render() const;
    void free();

private:
    GLuint vao;
    GLuint vbo;
    GLint posLocation;
    int numVertices;
};

#endif // _CIRCLE_INCLUDE

