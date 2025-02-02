#version 330 core

in vec4 fragData;
out vec4 FragColor;

void main() {
    vec3 Color = fragData.xyz;
    FragColor = vec4(Color, 1.0);
}