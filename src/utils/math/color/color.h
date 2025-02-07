#ifndef COLOR_H
#define COLOR_H

#include <glm/vec3.hpp>

inline glm::vec3 HEXtoRGB(const int hex) {
    int r = (hex >> 16) & 0xFF;
    int g = (hex >> 8) & 0xFF;
    int b = hex & 0xFF;

    return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
}

inline int RGBtoHex(const glm::vec3 RGB) {
    int r = static_cast<int>(RGB.r * 255.0f);
    int g = static_cast<int>(RGB.g * 255.0f);
    int b = static_cast<int>(RGB.b * 255.0f);

    return (r << 16) | (g << 8) | b;
}

#endif