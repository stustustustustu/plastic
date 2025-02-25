#include "UpgradePanel.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

UpgradePanel::UpgradePanel(const Upgrade& upgrade, glm::vec2 position, glm::vec2 size) : upgrade(upgrade), position(position), size(size),
      button(position + glm::vec2(size.x - (border + 48), 2 * border + 20), glm::vec2(48, 24), "Buy") {

      button.addCallback([this]() {
            if (game -> player -> getCoins() >= this -> upgrade.getCost()) {
                  game -> player -> spendCoins(this -> upgrade.getCost());
                  //game -> player -> applyUpgrade(this -> upgrade);
            }
      });
}

void UpgradePanel::render() {
      // panel shadows
      game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(6, 6), 2, 32, 32, size - glm::vec2(border, border), 0, HEXtoRGB(0x000000));

      // panels
      game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(4, 4), 2, 32, 32, size - glm::vec2(border, border), 0, HEXtoRGB(0x4F4F4F));

      // button shadows
      game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(size.x - (border + 48) + 2, 2 * border + 22), 2, 32, 32, glm::vec2(48, 24), 0, HEXtoRGB(0x000000));

      // button
      int buttonColor = game -> player -> getCoins() >= upgrade.getCost() ? 0x6F6F6F : 0x3F3F3F;
      game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(size.x - (border + 48), 2 * border + 20), 2, 32, 32, glm::vec2(48, 24), 0, HEXtoRGB(buttonColor));
}

void UpgradePanel::update() {
      button.update();
}

void UpgradePanel::setPosition(glm::vec2 position) {
      this -> position = position;
      button.setPosition(position + glm::vec2(size.x - (border + 48), 2 * border + 20));
}

glm::vec2 UpgradePanel::getSize() const {
      return this -> size - glm::vec2(8);
}