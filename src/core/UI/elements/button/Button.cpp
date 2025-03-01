#include "Button.h"
#include "../src/Game.h"

const auto game = Game::getInstance();


void Button::render() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(2), 2, 32, 32, size, 0, HEXtoRGB(0x000000));
    if (!isActive()) {
        game -> renderer -> DrawSpriteSheet(*game -> texture, position, 2, 32, 32, size, 0, brightness(color, -0.50));
        return;
    }
    game -> renderer -> DrawSpriteSheet(*game -> texture, position, 2, 32, 32, size, 0, isHovering() ? brightness(color, 0.25) : color);
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