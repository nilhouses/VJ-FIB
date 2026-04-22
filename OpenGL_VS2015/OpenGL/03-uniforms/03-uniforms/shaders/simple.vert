#version 330

in vec2 position;
uniform vec2 scale; // [Nil] nou uniform

void main()
{
	gl_Position = vec4(scale*position, 0.0, 1.0);
}
