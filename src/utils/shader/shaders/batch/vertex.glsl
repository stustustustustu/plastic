#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexIndex;

out vec2 texIndex;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos, 0.0, 1.0);
    texIndex = aTexIndex;
}