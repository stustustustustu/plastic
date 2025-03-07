#include "Textbox.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

Textbox::Textbox(glm::vec2 position, glm::vec2 size, const std::string& placeholder)
    : Element(position, size), placeholder(placeholder) {}

void Textbox::render() {
    glm::vec3 color = focused ? HEXtoRGB(0x5F5F5F) : HEXtoRGB(0x3F3F3F);

    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(2), 2, 32, 32, size, 0, HEXtoRGB(0x000000));

    game -> renderer -> DrawSpriteSheet(*game -> texture, position, 2, 32, 32, size, 0, color);

    std::string displayText = text.empty() ? placeholder : text;
    game -> renderer -> DrawText(displayText, getCenteredTextPosition(displayText, 16.0f) + glm::vec2(0, 14), 16.0f, true, text.empty() ? HEXtoRGB(0xAFAFAF) : HEXtoRGB(0xFFFFFF));
}

void Textbox::update() {
    if (isClicked() && Element::debounce(0.25f)) {
        focused = !focused;
    } else if (!isHovering() && glfwGetMouseButton(game -> window, GLFW_MOUSE_BUTTON_1)) {
        focused = false;
    }

    if (focused) {
        if (glfwGetKey(game -> window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && debounce(GLFW_KEY_BACKSPACE, 0.1)) {
            if (!text.empty()) {
                text.pop_back();
            }
        }

        if (glfwGetKey(game -> window, GLFW_KEY_SPACE) == GLFW_PRESS && debounce(GLFW_KEY_SPACE, 0.25)) {
            text += ' ';
        }

        if (glfwGetKey(game -> window, GLFW_KEY_ENTER) == GLFW_PRESS && debounce(GLFW_KEY_ENTER, 0.25)) {
            focused = false;
            executeAction();
        }

        for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; ++key) {
            if (glfwGetKey(game -> window, key) == GLFW_PRESS && debounce(key, 0.25)) {
                char c = 'a' + (key - GLFW_KEY_A);
                if (glfwGetKey(game -> window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(game -> window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
                    c = toupper(c);
                }
                text += c;
            }
        }

        for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; ++key) {
            if (glfwGetKey(game -> window, key) == GLFW_PRESS && debounce(key, 0.25)) {
                char c = '0' + (key - GLFW_KEY_0);
                text += c;
            }
        }
    }
}

std::string Textbox::getText() const {
    return this -> text;
}

void Textbox::setText(const std::string& text) {
    this -> text = text;
}

bool Textbox::debounce(int key, float debounceTime) {
    auto now = std::chrono::high_resolution_clock::now();

    if (keyLastPressTimes.find(key) != keyLastPressTimes.end()) {
        auto lastPressTime = keyLastPressTimes[key];
        float timeSinceLastPress = std::chrono::duration<float>(now - lastPressTime).count();

        if (timeSinceLastPress < debounceTime) {
            return false;
        }
    }

    keyLastPressTimes[key] = now;
    return true;
}