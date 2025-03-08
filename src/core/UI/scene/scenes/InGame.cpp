#include "InGame.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

InGame::InGame() : Scene("IN_GAME"),
                    turretShopToggle(glm::vec2(game -> getSize().at(0) - 2 - portrait / 2, portrait), glm::vec2(portrait / 2, portrait / 2), "", HEXtoRGB(0x2F2F2F)),
                    portraitToggle(glm::vec2(border, border), glm::vec2(portrait, portrait), "", HEXtoRGB(0x2F2F2F)),
                    playerShopToggle(glm::vec2(border + border / 2, portrait + (border + border / 2)), glm::vec2(portrait / 2, portrait / 2), "", HEXtoRGB(0x2F2F2F)),
                    turretUpgradeClose(glm::vec2(0, 0), glm::vec2(24), "", HEXtoRGB(0x3F3F3F)) {

    portraitToggle.addCallback([this]() {
        isAdvancedView = !isAdvancedView;
    });

    playerShopToggle.addCallback([this]() {
        isPlayerShopOpen = !isPlayerShopOpen;
    });

    turretShopToggle.addCallback([this]() {
        isTurretShopOpen = !isTurretShopOpen;
    });

    turretUpgradeClose.addCallback([this]() {
        if (game -> getCurrentWorld()) {
            game -> getCurrentWorld() -> turret -> closeUpgradeMenu();
        }
    });

    if (game -> getCurrentWorld()) {
        refreshUpgradePanels();
    }

    glm::vec2 panelSize(2 * portrait + border + width, 6 * portrait / 3 - border);
    glm::vec2 panelPosition(game -> getSize().at(0) - (2 * portrait + border + width) - 4, portrait);

    shopPanels.push_back(std::make_unique<ShopPanel>(panelPosition, panelSize, TurretType::LASER));
    shopPanels.push_back(std::make_unique<ShopPanel>(panelPosition + glm::vec2(0, panelSize.y + border), panelSize, TurretType::RIFLE));
    shopPanels.push_back(std::make_unique<ShopPanel>(panelPosition + glm::vec2(0, 2 * (panelSize.y + border)), panelSize, TurretType::BOMB));
}

void InGame::resize() {
    glm::vec2 panelSize(2 * portrait + border + width, 6 * portrait / 3 - border);
    glm::vec2 panelPosition(game -> getSize().at(0) - (2 * portrait + border + width) - 4, portrait);

    for (int i = 0; i < shopPanels.size(); ++i) {
        shopPanels[i] -> setPosition(panelPosition + glm::vec2(0, i * (panelSize.y + border)));
    }
}

void InGame::render() {
    renderPlayerStats();
    renderWaveInfo();

    if (isPlayerShopOpen) {
        renderPlayerShop();
    }

    renderTurretShop();
    renderTurretUpgrades();
}

void InGame::update() {
    portraitToggle.update();
    playerShopToggle.update();

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

    if (isPlayerShopOpen) {
        for (size_t i = 0; i < upgradePanels.size(); ++i) {
            glm::vec2 panelPosition = isAdvancedView
                ? glm::vec2(4, 2 * (portrait + border) + 2 + i * (upgradePanels[i] -> getSize().y + border))
                : glm::vec2(4, 2 * border + portrait + portrait / 2 + i * (upgradePanels[i] -> getSize().y + border));
            upgradePanels[i] -> setPosition(panelPosition);
            upgradePanels[i] -> update();
        }
    }

    if (isTurretShopOpen) {
        for (auto &panel : shopPanels) {
            panel -> update();
        }
        turretShopToggle.setPosition(glm::vec2(game -> getSize().at(0) - (2 * portrait + border + width) - 4 - portrait / 2, portrait));
    } else {
        turretShopToggle.setPosition(glm::vec2(game -> getSize().at(0) - 2 - portrait / 2, portrait));
    }

    if (game -> getCurrentWorld() -> turret -> isUpgrading()) {
        turretUpgradeClose.setPosition(game -> getCurrentWorld() -> turret -> menuPosition + glm::vec2(game -> getCurrentWorld() -> turret -> menuSize.x - (2 * border + game -> text -> GetWidth("X", 16)), border - 4));
        turretUpgradeClose.update();
    }

    turretShopToggle.update();
}

void InGame::renderPlayerStats() {
    if (isAdvancedView) {
        renderAdvancedStats();
    } else {
        std::string coinText = "$" + std::to_string(game -> getCurrentWorld() -> player -> getCoins());
        int coinWidth = game -> text -> GetWidth(coinText, 16.0f) + border / 2;

        // background shadows
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4) + glm::vec2(2), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6) + glm::vec2(2), 2, 32, 32, glm::vec2(width + 4, 34), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 40) + glm::vec2(2), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border + border / 2, portrait + (border + border / 2)) + glm::vec2(2), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

        // backgrounds
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6), 2, 32, 32, glm::vec2(width + 4, 34), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 40), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border + border / 2, portrait + (border + border / 2)), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));

        // bar background
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 10), 2, 32, 32, glm::vec2(width, 12), 0, HEXtoRGB(0x191919));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 24), 2, 32, 32, glm::vec2(width, 12), 0, HEXtoRGB(0x191919));

        // portrait
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border), 2, 32, 32, glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFF));

        // health
        float health = game -> getCurrentWorld() -> player -> getHealth() / game -> getCurrentWorld() -> player -> getMaxHealth();
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 10), 2, 32, 32, glm::vec2(width * health, 12), 0, HEXtoRGB(0xC04C47));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 20), 2, 32, 32, glm::vec2(width * health, 2), 0, HEXtoRGB(0x963531));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2) + width * health - 2, 10), 2, 32, 32, glm::vec2(2, 12), 0, HEXtoRGB(0x963531));

        // shield
        float shield = game -> getCurrentWorld() -> player -> getShield() / game -> getCurrentWorld() -> player -> getMaxShield();
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 24), 2, 32, 32, glm::vec2(width * shield, 12), 0, HEXtoRGB(0x6699AA));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 34), 2, 32, 32, glm::vec2(width * shield, 2), 0, HEXtoRGB(0x426977));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2) + width * shield - 2, 24), 2, 32, 32, glm::vec2(2, 12), 0, HEXtoRGB(0x426977));

        // coins
        game -> renderer -> DrawText(coinText, glm::vec2(portrait + (border + border / 2), portrait + border), 16.0f, true, HEXtoRGB(0xFDFF74));

        // popups
        if (isHoveringHealth) {
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(1) << game -> getCurrentWorld() -> player -> getHealth() << " / " << game -> getCurrentWorld() -> player -> getMaxHealth();

            renderPopup(
                stream.str(),
                game -> input -> getMousePosition() + glm::vec2(12, 4),
                HEXtoRGB(0xC04C47)
            );
        } else if (isHoveringShield) { // cant hover both
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(1) << game -> getCurrentWorld() -> player -> getShield() << " / " << game -> getCurrentWorld() -> player -> getMaxShield();

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
    healthStream << std::fixed << std::setprecision(1) << game -> getCurrentWorld() -> player -> getHealth() << " / " << game -> getCurrentWorld() -> player -> getMaxHealth();
    shieldStream << std::fixed << std::setprecision(1) << game -> getCurrentWorld() -> player -> getShield() << " / " << game -> getCurrentWorld() -> player -> getMaxShield();
    damageStream << game -> getCurrentWorld() -> player -> getDamage();
    speedStream << game -> getCurrentWorld() -> player -> getSpeed();

    coinStream << "$" << game -> getCurrentWorld() -> player -> getCoins();

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
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border + border / 2, portrait + (border + border / 2)) + glm::vec2(2), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

    // backgrounds
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(4, 4), 2, 32, 32, glm::vec2(portrait + border), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 6), 2, 32, 32, glm::vec2(textWidth + (2 * border), 84), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(portrait + (border + border / 2), 90), 2, 32, 32, glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(border + border / 2, portrait + (border + border / 2)), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));

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

void InGame::renderPlayerShop() {
    glm::vec2 pos;
    glm::vec2 size(2 * portrait + border + width, 8 * portrait);
    isAdvancedView ? pos = glm::vec2(4, 2 * (portrait + border) + 2) : pos = glm::vec2(4, 2 * border + portrait + portrait / 2);

    // background shadows
    game -> renderer -> DrawSpriteSheet(*game -> texture, pos + glm::vec2(2), 2, 32, 32, size, 0, HEXtoRGB(0x000000));

    // backgrounds
    game -> renderer -> DrawSpriteSheet(*game -> texture, pos, 2, 32, 32, size, 0, HEXtoRGB(0x2F2F2F));

    // upgrade panels
    for (auto &panel : upgradePanels) {
        panel -> render();
    }
}

void InGame::renderTurretShop() {
    auto screenWidth = game -> getSize().at(0);
    glm::vec2 size(2 * portrait + border + width, 6 * portrait);

    if (isTurretShopOpen) {
        // background shadows
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(screenWidth - size.x - 4, portrait) + glm::vec2(2), 2, 32, 32, size, 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(screenWidth - size.x - 4 - portrait / 2, portrait) + glm::vec2(2), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

        // backgrounds
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(screenWidth - size.x - 4, portrait), 2, 32, 32, size, 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(screenWidth - size.x - 4 - portrait / 2, portrait), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));

        // turret image
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(screenWidth - size.x, portrait + 4) + glm::vec2(4, 4), 2, 32, 32, glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFFF));

        for (auto &panel : shopPanels) {
            panel -> render();
        }
    } else {
        // background shadows
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(screenWidth - 2 - portrait / 2, portrait) + glm::vec2(2), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

        // backgrounds
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(screenWidth - 2 - portrait / 2, portrait), 2, 32, 32, glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));
    }
}

void InGame::renderTurretUpgrades() {
    auto turret = game -> getCurrentWorld() -> turret -> getSelectedTurret();
    if (!turret && !game -> getCurrentWorld() -> turret -> isUpgrading()) return;

    auto popupPosition = game -> getCurrentWorld() -> turret -> menuPosition;
    auto popupSize = game -> getCurrentWorld() -> turret -> menuSize;

    // shadow
    game -> renderer -> DrawSpriteSheet(*game -> texture, popupPosition + glm::vec2(2), 2, 32, 32, popupSize, 0.0f, HEXtoRGB(0x000000));

    // background
    game -> renderer -> DrawSpriteSheet(*game -> texture, popupPosition, 2, 32, 32, popupSize, 0.0f, HEXtoRGB(0x2F2F2F));

    // X button
    turretUpgradeClose.render();
    game -> renderer -> DrawText("X", popupPosition + glm::vec2(popupSize.x - (border + game -> text -> GetWidth("X", 16) + 1), border + 14), 16.0f, true, HEXtoRGB(0xFF0000));

    // skill tree
    auto skillTree = game -> getCurrentWorld() -> turret -> getUpgradeManager().getSkillTree();
    auto nodePositions = game -> getCurrentWorld() -> turret -> calculateNodePositions();

    for (const auto& [upgrade, prereqs] : skillTree) {
        glm::vec2 nodePosition = nodePositions[upgrade];

        renderUpgradeNode(upgrade, nodePosition, HEXtoRGB(0x4F4F4F));

        for (auto prereq : prereqs) {
            glm::vec2 prereqPosition = nodePositions[prereq];
            renderConnection(nodePosition + glm::vec2(12), prereqPosition + glm::vec2(12), HEXtoRGB(0xFFFFFF));
        }
    }
}

void InGame::renderWaveInfo() {
    game -> renderer -> DrawText("Enemies alive: " + std::to_string(game -> getCurrentWorld() -> wave -> getCurrentEnemies() -> size()), glm::vec2((game -> getSize().at(0) / 2) - 50, 40), 24.0f, true);
}

void InGame::renderPopup(const std::string &text, const glm::vec2 &position, const glm::vec3 &color) {
    int textWidth = game -> text -> GetWidth(text, 16.0f);
    int textHeight = 16;

    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(2), 2, 32, 32, glm::vec2(textWidth + 10, textHeight + 10), 0, HEXtoRGB(0x000000)); // shadow
    game -> renderer -> DrawSpriteSheet(*game -> texture, position, 2, 32, 32, glm::vec2(textWidth + 10, textHeight + 10), 0, HEXtoRGB(0x2F2F2F));

    game -> renderer -> DrawText(text, position + glm::vec2(5, textHeight + 2), 16.0f, true, color);
}

void InGame::renderUpgradeNode(Upgrade *upgrade, const glm::vec2 &position, const glm::vec3 &color) {
    Button button(position, glm::vec2(24), "", color);

    button.render();
    if (button.isHovering()) {
        renderPopup(upgrade -> getDescription(), game -> input -> getMousePosition() + glm::vec2(12, 4), glm::vec3(1.0f, 1.0f, 1.0f));
    }
}

void InGame::renderConnection(const glm::vec2 &from, const glm::vec2 &to, const glm::vec3 &color) {
    game -> renderer -> DrawLine(from, to, 2.0f, color);
}

void InGame::refreshUpgradePanels() {
    upgradePanels.clear();

    glm::vec2 panelSize(2 * portrait + border + width, 10 * portrait / 5);

    for (const auto& path : game -> getCurrentWorld() -> upgrade -> getPaths()) {
        UpgradePath upgradePath = path.second;

        const Upgrade& nextUpgrade = upgradePath.getNextUpgrade();
        glm::vec2 panelPosition = glm::vec2(4, 2 * border + portrait + portrait / 2) + glm::vec2(0, upgradePanels.size() * panelSize.y / 5);

        upgradePanels.push_back(std::make_unique<UpgradePanel>(
            nextUpgrade,
            panelPosition,
            panelSize,
            [this]() { refreshUpgradePanels(); }
        ));
    }
}
