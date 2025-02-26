#include "UpgradePanel.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

UpgradePanel::UpgradePanel(const Upgrade &upgrade, glm::vec2 position, glm::vec2 size, Refresh refresh)
    : upgrade(upgrade), position(position), size(size), refresh(refresh),
    button(position + glm::vec2(size.x - (border + 48), size.y - (24 + border)), glm::vec2(48, 24), "Buy") {

    button.addCallback([this, refresh, upgrade]() {
        if (game -> player -> getCoins() >= this -> upgrade.getCost()) {
            game -> upgrade -> upgrade(this -> upgrade.getType());
            game -> player -> applyUpgrade(upgrade);

            if (refresh) {
                refresh();
            }
        }
    });
}

void UpgradePanel::render() {
    // panel shadows
    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(6, 6), 2, 32, 32, size - glm::vec2(border, border), 0, HEXtoRGB(0x000000));

    // panels
    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(4, 4), 2, 32, 32, size - glm::vec2(border, border), 0, HEXtoRGB(0x4F4F4F));

    // text
    game -> renderer -> DrawText(upgrade.getName(), position + glm::vec2(border, border / 2 + 16), 16.0f, true, HEXtoRGB(0xFFFFFF));
    game -> renderer -> DrawText(upgrade.getDescription(), position + glm::vec2(border, border + 24), 8.0f, true, HEXtoRGB(0xDFDFDF));
    game -> renderer -> DrawText("$"+ std::to_string(upgrade.getCost()), position + glm::vec2(border, size.y - border - 2), 16.0f, true, HEXtoRGB(0xFDFF74));

    // button shadows
    game -> renderer -> DrawSpriteSheet(*game -> texture, button.getPosition() + glm::vec2(2, 2), 2, 32, 32, button.getSize(), 0, HEXtoRGB(0x000000));

    // button
    int buttonColor = game -> player -> getCoins() >= upgrade.getCost() ? 0x6F6F6F : 0x3F3F3F;
    game -> renderer -> DrawSpriteSheet(*game -> texture, button.getPosition(), 2, 32, 32, button.getSize(), 0, HEXtoRGB(buttonColor));
}

void UpgradePanel::update() {
    button.update();
}

void UpgradePanel::setPosition(glm::vec2 position) {
    this -> position = position;
    button.setPosition(position + glm::vec2(size.x - (border + 48), size.y - (24 + border)));
}

glm::vec2 UpgradePanel::getSize() const {
    return this -> size - glm::vec2(8);
}