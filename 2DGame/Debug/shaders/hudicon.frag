#version 330
uniform sampler2D tex;
uniform float fillAmount; // 1.0 (blanco) -> 0.0 (color original)

in vec2 texCoordFrag;
in float vNormalizedY; 
out vec4 outColor;

void main() {
    vec4 texColor = texture(tex, texCoordFrag);
    if(texColor.a < 0.01) discard;
    if (vNormalizedY > (1.0 - fillAmount)) outColor = mix(texColor, vec4(1.0, 1.0, 1.0, texColor.a), 0.7);
    else outColor = texColor;
}