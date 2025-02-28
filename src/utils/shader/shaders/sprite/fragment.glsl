#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float opacity;

void main() {
    vec4 texColor = texture(image, TexCoords);
    FragColor = vec4(spriteColor, opacity) * texColor;
}