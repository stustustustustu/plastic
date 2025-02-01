#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 fragData;

uniform mat4 model;

uniform bool useTexture;

void main() {
    if (!useTexture) {
        fragData = vec4(aColor, 1.0);
        gl_Position = vec4(aPos, 1.0);
    } else {
        fragData = vec4(aTexCoord, 0.0, 0.0);
        gl_Position = model * vec4(aPos, 0.0);
    }
}