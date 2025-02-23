#include "InGame.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void InGame::render() {
    renderPlayerStats();
    renderWaveInfo();
}

void InGame::update() {
    portraitButton.update();

    if (portraitButton.isClicked()) {
        isAdvancedView = !isAdvancedView;
    }

    if (!isAdvancedView) {
        auto mousePos = game -> input -> getMousePosition();

        auto healthBarPos = glm::vec2(portrait + (border + border / 2), 10);
        auto shieldBarPos = glm::vec2(portrait + (border + border / 2), 24);

        glm::vec2 healthBarHalfDim = glm::vec2(width / 2, 6);
        glm::vec2 shieldBarHalfDim = glm::vec2(width / 2, 6);

        isHoveringHealth = Collision::isPointInRectangle(
                    {mousePos.x, mousePos.y},
                    {healthBarPos.x + healthBarHalfDim.x, healthBarPos.y + healthBarHalfDim.y},
                    {healthBarHalfDim.x, healthBarHalfDim.y}
        );

        isHoveringShield = Collision::isPointInRectangle(
                            {mousePos.x, mousePos.y},
                            {shieldBarPos.x + shieldBarHalfDim.x, shieldBarPos.y + shieldBarHalfDim.y},
                            {shieldBarHalfDim.x, shieldBarHalfDim.y}
        );
    }
}

void InGame::renderPlayerStats() {
    if (isAdvancedView) {
        renderAdvancedStats();
    } else {
        std::string coinText = "$" + std::to_string(game -> player -> getCoins());
        int coinWidth = game -> text -> GetWidth(coinText, 16.0f) + border / 2;

        // background shadows
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4) + glm::vec2(2), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6) + glm::vec2(2), 2, 32, 32, glm::vec2(width + 4, 34), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 40) + glm::vec2(2), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x000000));

        // backgrounds
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6), 2, 32, 32, glm::vec2(width + 4, 34), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 40), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x2F2F2F));

        // bar background
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 10), 2, 32, 32, glm::vec2(width, 12), 0, HEXtoRGB(0x191919));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 24), 2, 32, 32, glm::vec2(width, 12), 0, HEXtoRGB(0x191919));

        // portrait
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border), 2, 32, 32, glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFF));

        // health
        float health = game -> player -> getHealth() / game -> player -> getMaxHealth();
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 10), 2, 32, 32, glm::vec2(width * health, 12), 0, HEXtoRGB(0xC04C47));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 20), 2, 32, 32, glm::vec2(width * health, 2), 0, HEXtoRGB(0x963531));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2) + width * health - 2, 10), 2, 32, 32, glm::vec2(2, 12), 0, HEXtoRGB(0x963531));

        // shield
        float shield = game -> player -> getShield() / game -> player -> getMaxShield();
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 24), 2, 32, 32, glm::vec2(width * shield, 12), 0, HEXtoRGB(0x6699AA));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 34), 2, 32, 32, glm::vec2(width * shield, 2), 0, HEXtoRGB(0x426977));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2) + width * shield - 2, 24), 2, 32, 32, glm::vec2(2, 12), 0, HEXtoRGB(0x426977));

        // coins
        game -> renderer -> DrawText(coinText, glm::vec2(portrait + (border + border / 2), portrait + border), 16.0f, true, HEXtoRGB(0xFDFF74));

        // popups
        if (isHoveringHealth) {
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(1) << game -> player -> getHealth() << " / " << game -> player -> getMaxHealth();

            renderPopup(
                stream.str(),
                game -> input -> getMousePosition() + glm::vec2(12, 4),
                HEXtoRGB(0xC04C47)
            );
        } else if (isHoveringShield) { // cant hover both
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(1) << game -> player -> getShield() << " / " << game -> player -> getMaxShield();

            renderPopup(
                stream.str(),
                game -> input -> getMousePosition() + glm::vec2(12, 4),
                HEXtoRGB(0x6699AA)
            );
        }
    }
}

void InGame::renderAdvancedStats() {
    // prep values / texts
    std::ostringstream healthStream, shieldStream, coinStream, damageStream, speedStream;
    healthStream << std::fixed << std::setprecision(1) << game -> player -> getHealth() << " / " << game -> player -> getMaxHealth();
    shieldStream << std::fixed << std::setprecision(1) << game -> player -> getShield() << " / " << game -> player -> getMaxShield();
    damageStream << game -> player -> getDamage();
    speedStream << game -> player -> getSpeed();

    coinStream << "$" << game -> player -> getCoins();

    auto textWidth = std::max(
        std::max(
            game -> text -> GetWidth(healthStream.str(), 16),
            game -> text -> GetWidth(shieldStream.str(), 16)),
        std::max(
            game -> text -> GetWidth(damageStream.str(), 16),
            game -> text -> GetWidth(speedStream.str(), 16))
    );

    int coinWidth = game -> text -> GetWidth(coinStream.str(), 16.0f) + border / 2 + 2;

    // background shadows
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4) + glm::vec2(2), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6) + glm::vec2(2), 2, 32, 32, glm::vec2(textWidth + (2 * border), 84), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 90) + glm::vec2(2), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x000000));

    // backgrounds
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6), 2, 32, 32, glm::vec2(textWidth + (2 * border), 84), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 90), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x2F2F2F));

    // portrait
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border), 2, 32, 32, glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFF));

    // stats
    glm::vec2 startPos = glm::vec2(portrait + (border + border / 2), border + 16);
    game -> renderer -> DrawText(healthStream.str(), startPos + glm::vec2(border, 0 * 20), 16.0f, true, HEXtoRGB(0xC04C47));
    game -> renderer -> DrawText(shieldStream.str(), startPos + glm::vec2(border, 1 * 20), 16.0f, true, HEXtoRGB(0x6699AA));
    game -> renderer -> DrawText(damageStream.str(), startPos + glm::vec2(border, 2 * 20), 16.0f, true, HEXtoRGB(0xFFFFFF));
    game -> renderer -> DrawText(speedStream.str(), startPos + glm::vec2(border, 3 * 20), 16.0f, true, HEXtoRGB(0xFFFFFF));

    // coins
    game -> renderer -> DrawText(coinStream.str(), glm::vec2(portrait + (2 * border), border + 98), 16.0f, true, HEXtoRGB(0xFDFF74));
}

void InGame::renderWaveInfo() {
    game -> renderer -> DrawText("Enemies alive " + std::to_string(game -> wave -> getCurrentEnemies() -> size()), glm::vec2((game -> getSize().at(0) / 2) - 50, 40), 24.0f, true);
}

void InGame::renderPopup(const std::string &text, const glm::vec2 &position, const glm::vec3 &color) {
    int textWidth = game -> text -> GetWidth(text, 16.0f);
    int textHeight = 16;

    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(2), 2, 32, 32, glm::vec2(textWidth + 10, textHeight + 10), 0, HEXtoRGB(0x000000)); // shadow
    game -> renderer -> DrawSpriteSheet(*game -> texture, position, 2, 32, 32, glm::vec2(textWidth + 10, textHeight + 10), 0, HEXtoRGB(0x2F2F2F));

    game -> renderer -> DrawText(text, position + glm::vec2(5, textHeight + 2), 16.0f, true, color);
}