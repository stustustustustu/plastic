#include "ShopPanel.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

constexpr auto border = 8;
constexpr auto portrait = 48;

ShopPanel::ShopPanel(glm::vec2 position, glm::vec2 size, TurretType type)
    : position(position), size(size),
      button(position + glm::vec2(border, portrait + border + 6), glm::vec2(24, 24), ""),
      type(type) {
    button.addCallback([this]() {
        game -> turret -> startPlacingTurret(this -> type);;
    });
}

void ShopPanel::render() {
    // panel shadow
    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(border / 2 + 2), 2, 32, 32, size - glm::vec2(border, 0), 0, HEXtoRGB(0x000000));

    // panel
    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(border / 2), 2, 32, 32, size - glm::vec2(border, 0), 0, HEXtoRGB(0x4F4F4F));

    // turret image
    int hex;
    switch (type) {
        case TurretType::LASER:
            hex = 0xFF5733;
        break;
        case TurretType::RIFLE:
            hex = 0xF2EE0A;
        break;
        case TurretType::BOMB:
            hex = 0x323232;
        break;
    }

    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(border, border), 2, 32, 32, glm::vec2(portrait), 0, HEXtoRGB(hex));

    // button
    game -> renderer -> DrawSpriteSheet(*game -> texture, button.getPosition() + glm::vec2(2), 2, 32, 32, button.getSize(), 0, HEXtoRGB(0x000000));

    // button
    game -> renderer -> DrawSpriteSheet(*game -> texture, button.getPosition(), 2, 32, 32, button.getSize(), 0, HEXtoRGB(0x6F6F6F));
}

void ShopPanel::update() {
    button.update();
}

void ShopPanel::setPosition(glm::vec2 position) {
    this->position = position;
    button.setPosition(position + glm::vec2(size.x - (border + 48), size.y - (24 + border)));
}

glm::vec2 ShopPanel::getSize() const {
    return this->size;
}