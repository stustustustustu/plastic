#include "Dropdown.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

Dropdown::Dropdown(glm::vec2 position, glm::vec2 size, const std::string &label) : Element(position, size) {
    options.push_back(label);
}

void Dropdown::render() {
    game -> renderer -> DrawRect(position + glm::vec2(2), size, 0, HEXtoRGB(0x000000));

    game -> renderer -> DrawRect(position, size, 0, HEXtoRGB(0x3F3F3F));

    game -> renderer -> DrawText(options[selectedIndex], getCenteredTextPosition(options[selectedIndex], 16.0f) + glm::vec2(0, 14), 16.0f, true, HEXtoRGB(0xFFFFFF));

    if (open) {
        for (size_t i = 0; i < options.size(); ++i) {
            glm::vec2 optionPosition = position + glm::vec2(0, size.y * i);

            game -> renderer -> DrawRect(optionPosition + glm::vec2(2), size, 0, HEXtoRGB(0x000000));

            game -> renderer -> DrawRect(optionPosition, size, 0, HEXtoRGB(0x3F3F3F));

            game -> renderer -> DrawText(options[i], getCenteredTextPosition(options[i], 16.0f) + glm::vec2(0, size.y * i) + glm::vec2(0, 14), 16.0f, true, HEXtoRGB(0xFFFFFF));
        }
    }
}

void Dropdown::update() {
    if (isClicked() && debounce(0.25f)) {
        open = !open;
    }

    if (open) {
        auto mousePos = game -> input -> getMousePosition();

        for (size_t i = 0; i < options.size(); ++i) {
            glm::vec2 optionPosition = position + glm::vec2(0, size.y * i);
            if (mousePos.x >= optionPosition.x && mousePos.x <= optionPosition.x + size.x &&
                mousePos.y >= optionPosition.y && mousePos.y <= optionPosition.y + size.y) {
                if (glfwGetMouseButton(game -> window, GLFW_MOUSE_BUTTON_1) && debounce(0.25f)) {
                    selectOption(i);
                    open = false;
                }
            }
        }
    }
}

void Dropdown::addOption(const std::string& option) {
    options.push_back(option);
}

int Dropdown::getOptionCount() const {
    return options.size();
}

int Dropdown::getSelectedIndex() const {
    return selectedIndex;
}

std::string Dropdown::getSelectedOption() const {
    return options[selectedIndex];
}

void Dropdown::selectOption(int index) {
    if (index >= 0 && index < options.size()) {
        selectedIndex = index;
    }
}

bool Dropdown::isOpen() const {
    return this -> open;
}
