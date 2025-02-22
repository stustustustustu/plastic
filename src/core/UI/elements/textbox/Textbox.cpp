#include "Textbox.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void Textbox::render() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, getPosition(), 2, 32, 32, size, 0, HEXtoRGB(0x646464));

    game -> renderer -> DrawText(text, getPosition() + glm::vec2(10, 10), 20.0f, true, HEXtoRGB(0xFFFFFF));
}

void Textbox::setText(const std::string &text) {
    this -> text = text;
}

std::string Textbox::getText() const {
    return this -> text;
}
