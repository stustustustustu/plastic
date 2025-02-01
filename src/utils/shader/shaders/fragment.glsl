#version 330 core

in vec4 fragData;
out vec4 FragColor;

uniform sampler2D spriteTexture;
uniform sampler2D maskTexture;
uniform vec3 spriteColor;

uniform bool useMask;
uniform bool useTexture;

void main() {
    if (useTexture) {
        vec2 TexCoords = fragData.xy;
        vec4 sprite = texture(spriteTexture, TexCoords);
        if (useMask) {
            vec4 mask = texture(maskTexture, TexCoords);
            FragColor = vec4(spriteColor, mask.r) * sprite;
        } else {
            FragColor = vec4(spriteColor, 1.0) * sprite;
        }
    } else {
        vec3 Color = fragData.xyz;
        FragColor = vec4(Color, 1.0);
    }
}