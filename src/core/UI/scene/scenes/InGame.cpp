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
    static int border = 8;
    static int portrait = 48;
    static int width = 100;

    std::string coinText = "$" + std::to_string(game -> player -> getCoins());
    int coinWidth = game -> text -> GetWidth(coinText, 16.0f) + border / 2;

    // backgrounds
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x1E1E1E));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6), 2, 32, 32, glm::vec2(width + 4, 34), 0, HEXtoRGB(0x1E1E1E));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 40), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x1E1E1E));

    // portrait
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border), 2, 32, 32, glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFF));

    // health
    float health = game -> player -> getHealth() / game -> player -> getMaxHealth();
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 10), 2, 32, 32, glm::vec2(width * health, 12), 0, HEXtoRGB(0xC04C47));

    // shield
    float shield = game -> player -> getShield() / game -> player -> getMaxShield();
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 24), 2, 32, 32, glm::vec2(width * shield, 12), 0, HEXtoRGB(0x6699AA));

    // coins
    game -> renderer -> DrawText(coinText, glm::vec2(portrait + (border + border / 2), portrait + border - 2), 16.0f, true);
}

void InGame::renderWaveInfo() {
    game -> renderer -> DrawText("Enemies alive " + std::to_string(game -> wave -> getCurrentEnemies() -> size()), glm::vec2((game -> getSize().at(0) / 2) - 50, 40), 24.0f, true);
}
