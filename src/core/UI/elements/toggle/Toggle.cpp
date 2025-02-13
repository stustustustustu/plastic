#include "Toggle.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void Toggle::render() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, position, 2, 32, 32, size, 0, HEXtoRGB(toggled ? 0x00FF00 : 0xFF0000));
    game -> renderer -> DrawSpriteSheet(*game -> texture, (toggled ? position + size - glm::vec2(size.y, size.y) : position) + glm::vec2(3, 3), 2, 32, 32, {size.y - 6, size.y - 6}, 0, HEXtoRGB(0xFFFFFF));
}

void Toggle::update() {
    toggled = isClicked() ? !toggled : toggled;
    hovered = isHovering();
}