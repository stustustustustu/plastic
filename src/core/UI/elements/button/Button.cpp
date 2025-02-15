#include "Button.h"
#include "../src/Game.h"

const auto game = Game::getInstance();


void Button::render() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, !clicked ? position : position + glm::vec2(0, 2), 2, 32, 32, size, 0, HEXtoRGB(!hovered ? 0x323232 : 0x646464));
}

void Button::update() {
    clicked = isClicked();
    hovered = isHovering();
}