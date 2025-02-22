#include "InGame.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void InGame::render() {
    renderPlayerStats();
    renderWaveInfo();
}

void InGame::update() {
}

void InGame::renderPlayerStats() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4), 2, 32, 32, glm::vec2(130, 40), 0, HEXtoRGB(0x323232));

    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(30, 8), 2, 32, 32, glm::vec2(1 * game -> player -> getHealth(), 20), 0, HEXtoRGB(0xFF0000));

    game -> renderer -> DrawText(std::to_string(static_cast<int>(game -> player -> getHealth())) + " / " + std::to_string(static_cast<int>(game -> player -> getMaxHealth())), glm::vec2(10, 30), 24.0f, true);
    game -> renderer -> DrawText(std::to_string(game -> player -> getCoins()) + " $", glm::vec2(10, 54), 24.0f, true);
}

void InGame::renderWaveInfo() {
    game -> renderer -> DrawText("Enemies alive " + std::to_string(game -> wave -> getCurrentEnemies() -> size()), glm::vec2((game -> getSize().at(0) / 2) - 50, 40), 24.0f, true);
}
