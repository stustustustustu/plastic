#include "Slider.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void Slider::render() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, getPosition(), 2, 32, 32, getSize(), 0, HEXtoRGB(0x323232));

    float normalizedValue = (value - min) / (max - min); // normalize to [0, 1]
    glm::vec2 thumbPosition = getPosition() + glm::vec2(normalizedValue * getSize().x, 0);

    game -> renderer -> DrawSpriteSheet(*game -> texture, thumbPosition, 2, 32, 32, {10, getSize().y}, 0, HEXtoRGB(0x646464));
}

void Slider::update() {
    if (isClicked()) {
        dragging = true;
    }

    if (dragging) {
        if (glfwGetMouseButton(game -> window, 0) == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(game -> window, &mouseX, &mouseY);

            float normalizedValue = (mouseX - getPosition().x) / getSize().x;
            normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue)); // clamp to [0, 1]

            value = min + normalizedValue * (max - min);

            if (rounding > 0) {
                value = std::round(value / rounding) * rounding;
            }

            value = std::max(min, std::min(max, value));
        } else {
            dragging = false;
        }
    }
}

float Slider::getValue() const {
    return this -> value;
}