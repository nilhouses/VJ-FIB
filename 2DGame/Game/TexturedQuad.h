#ifndef _TEXTURED_QUAD_INCLUDE
#define _TEXTURED_QUAD_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


class TexturedQuad
{

private:
	TexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

public:
	// Textured quads can only be created inside an OpenGL context
	static TexturedQuad *createTexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

	void render(const Texture &tex) const;
	void render() const;
	void free();

private:
	GLuint vao = GLuint();
	GLuint vbo = GLuint();
	GLint posLocation = GLint(), texCoordLocation = GLint();

};


#endif // _TEXTURED_QUAD_INCLUDE

