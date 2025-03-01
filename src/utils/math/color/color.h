#ifndef COLOR_H
#define COLOR_H

#include <glm/vec3.hpp>

inline glm::vec3 HEXtoRGB(const int HEX) {
    int r = (HEX >> 16) & 0xFF;
    int g = (HEX >> 8) & 0xFF;
    int b = HEX & 0xFF;

    return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
}

inline int RGBtoHex(const glm::vec3 RGB) {
    int r = static_cast<int>(RGB.r * 255.0f);
    int g = static_cast<int>(RGB.g * 255.0f);
    int b = static_cast<int>(RGB.b * 255.0f);

    return (r << 16) | (g << 8) | b;
}

inline glm::vec3 brightness(const glm::vec3 &color, float factor) {
    factor = glm::clamp(factor, -1.0f, 1.0f);

    glm::vec3 white = HEXtoRGB(0xFFFFFF);
    glm::vec3 black = HEXtoRGB(0x000000);

    if (factor > 0) {
        return glm::mix(color, white, factor);
    } else {
        return glm::mix(color, black, -factor);
    }
}

inline glm::vec3 brightness(int HEX, float factor) {
    return brightness(HEXtoRGB(HEX), factor);
}

#endif