#version 330 core

in vec2 texIndex;
out vec4 color;

uniform sampler2D texture;

void main() {
    color = texture(texture, texIndex);
}