#include "Button.h"
#include "../src/Game.h"

const auto game = Game::getInstance();


void Button::render() {
    game -> renderer -> DrawRect(position + glm::vec2(2), size, 0, HEXtoRGB(0x000000));
    if (!isActive()) {
        game -> renderer -> DrawRect(position, size, 0, brightness(color, -0.50));
        return;
    }

    game -> renderer -> DrawRect(position, size, 0, isHovering() ? brightness(color, 0.25) : color);

    game -> renderer -> DrawText(label, getCenteredTextPosition(label, 16.0f) + glm::vec2(0, 14), 16.0f, true);
}

void Button::update() {
    hovered = isHovering();

    if (isClicked() && debounce(0.25f)) {
        clicked = true;
        executeAction();
    } else {
        clicked = false;
    }
}