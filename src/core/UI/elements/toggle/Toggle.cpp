#include "Toggle.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void Toggle::render() {
    game -> renderer -> DrawRect(position, size, 0, HEXtoRGB(toggled ? 0x00FF00 : 0xFF0000));
    game -> renderer -> DrawRect((toggled ? position + size - glm::vec2(size.y, size.y) : position) + glm::vec2(3, 3), {size.y - 6, size.y - 6}, 0, HEXtoRGB(0xFFFFFF));
}

void Toggle::update() {
    hovered = isHovering();

    if (isClicked() && debounce(0.25f)) {
        toggled = !toggled;
        if (toggled) {
            executeAction();
        }
    }
}