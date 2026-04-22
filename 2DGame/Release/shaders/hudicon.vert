#version 330
uniform mat4 projection, modelview;
uniform vec2 texCoordDispl;

in vec2 position; // En píxeles [0 -> 32]
in vec2 texCoord;

out vec2 texCoordFrag;
out float vNormalizedY; // [0.0, 1.0]

void main()
{
    texCoordFrag = texCoord + texCoordDispl;
    vNormalizedY = position.y / 32.0; 

    gl_Position = projection * modelview * vec4(position, 0.0, 1.0);
}