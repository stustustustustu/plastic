#include "Slider.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void Slider::render() {
    game -> renderer -> DrawRect(position + glm::vec2(2), size, 0, HEXtoRGB(0x000000));

    game -> renderer -> DrawRect(position, size, 0, HEXtoRGB(0x3F3F3F));

    float normalizedValue = (value - min) / (max - min);
    glm::vec2 thumbPosition = position + glm::vec2(normalizedValue * size.x - 5, -2);

    game -> renderer -> DrawRect(thumbPosition + glm::vec2(2), {10, size.y + 4}, 0, HEXtoRGB(0x000000));

    game -> renderer -> DrawRect(thumbPosition, {10, size.y + 4}, 0, HEXtoRGB(0x646464));
}

void Slider::update() {
    if (isClicked()) {
        dragging = true;
    }

    if (dragging) {
        if (glfwGetMouseButton(game -> window, 0) == GLFW_PRESS) {
            auto mousePos = game -> input -> getMousePosition();

            float normalizedValue = (mousePos.x - position.x) / size.x;
            normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue));

            value = min + normalizedValue * (max - min);

            if (rounding > 0) {
                value = std::round(value / rounding) * rounding;
            }

            value = std::max(min, std::min(max, value));

            executeAction();
        } else {
            dragging = false;
        }
    }
}

float Slider::getValue() const {
    return this -> value;
}

void Slider::setValue(float value) {
    this -> value = std::max(min, std::min(max, value));
}

void Slider::setConstraints(float min, float max, float rounding) {
    this -> min = min;
    this -> max = max;
    this -> rounding = rounding;
}