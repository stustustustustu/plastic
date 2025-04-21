#include "InGame.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

InGame::InGame() : Scene("IN_GAME"),
                    turretShopToggle(glm::vec2(game -> getSize().x - 2 - portrait / 2, portrait), glm::vec2(portrait / 2, portrait / 2), "", HEXtoRGB(0x2F2F2F)),
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
    glm::vec2 panelPosition(game -> getSize().x - (2 * portrait + border + width) - 4, portrait);

    shopPanels.push_back(std::make_unique<ShopPanel>(panelPosition, panelSize, TurretType::LASER));
    shopPanels.push_back(std::make_unique<ShopPanel>(panelPosition + glm::vec2(0, panelSize.y + border), panelSize, TurretType::RIFLE));
    shopPanels.push_back(std::make_unique<ShopPanel>(panelPosition + glm::vec2(0, 2 * (panelSize.y + border)), panelSize, TurretType::BOMB));
}

void InGame::resize() {
    refreshUpgradePanels();

    glm::vec2 panelSize(2 * portrait + border + width, 6 * portrait / 3 - border);
    glm::vec2 panelPosition(game -> getSize().x - (2 * portrait + border + width) - 4, portrait);

    for (int i = 0; i < shopPanels.size(); ++i) {
        shopPanels[i] -> setPosition(panelPosition + glm::vec2(0, i * (panelSize.y + border)));
    }
}

void InGame::render() {
    renderPlayerStats();
    renderWaveInfo();
    renderScore();

    if (isPlayerShopOpen) {
        renderPlayerShop();
    }

    if (hoveredEnemy) {
        renderEnemyPopups();
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
                ? glm::vec2(4, 2 * (portrait + border) + 2 + i * (upgradePanels[i] -> getSize().y + border) - 21)
                : glm::vec2(4, 2 * border + portrait + portrait / 2 + i * (upgradePanels[i] -> getSize().y + border));
            upgradePanels[i] -> setPosition(panelPosition);
            upgradePanels[i] -> update();
        }
    }

    if (isTurretShopOpen) {
        for (auto &panel : shopPanels) {
            panel -> update();
        }
        turretShopToggle.setPosition(glm::vec2(game -> getSize().x - (2 * portrait + border + width) - 4 - portrait / 2, portrait));
    } else {
        turretShopToggle.setPosition(glm::vec2(game -> getSize().x - 2 - portrait / 2, portrait));
    }

    if (game -> getCurrentWorld() -> turret -> isUpgrading()) {
        turretUpgradeClose.setPosition(game -> getCurrentWorld() -> turret -> menuPosition + glm::vec2(game -> getCurrentWorld() -> turret -> menuSize.x - (2 * border + game -> text -> GetWidth("X", 16)), border - 4));
        turretUpgradeClose.update();
    }

    turretShopToggle.update();

    if (game -> getCurrentWorld()) {
        canForceNextWave = game -> getCurrentWorld() -> wave -> isActive() &&
                           game -> getCurrentWorld() -> wave -> getCurrentEnemies() &&
                           game -> getCurrentWorld() -> wave -> getCurrentEnemies() -> size() > 0;
    }

    hoveredEnemy = nullptr;
    if (game -> getCurrentWorld() && game -> getCurrentWorld() -> wave -> getCurrentEnemies()) {
        auto mousePos = game -> camera -> screenToWorld(game -> input -> getMousePosition());
        for (const auto& enemy : *game -> getCurrentWorld() -> wave -> getCurrentEnemies()) {
            if (Collision::isPointInRectangle(mousePos, enemy.getPosition() + enemy.getSizeScale() * 8, glm::vec2(enemy.getSizeScale() * 16))) {
                hoveredEnemy = &enemy;
            }
            if (Collision::lineRectangleIntersection(game -> getCurrentWorld() -> player -> getPosition() + glm::vec2(16), mousePos, enemy.getPosition() + enemy.getSizeScale() * 8, glm::vec2(enemy.getSizeScale() * 16)) &&
                glfwGetMouseButton(game  ->  window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
                hoveredEnemy = &enemy;
            }
        }
    }
}

void InGame::renderPlayerStats() {
    if (isAdvancedView) {
        renderAdvancedStats();
    } else {
        std::string coinText = "$" + std::to_string(game -> getCurrentWorld() -> player -> getCoins());
        int coinWidth = game -> text -> GetWidth(coinText, 16.0f) + border / 2;

        // background shadows
        game -> renderer -> DrawRect(glm::vec2(4, 4) + glm::vec2(2), glm::vec2(portrait + border), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 6) + glm::vec2(2), glm::vec2(width + 4, 34), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 40) + glm::vec2(2), glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawRect(glm::vec2(border + border / 2, portrait + (border + border / 2)) + glm::vec2(2), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

        // backgrounds
        game -> renderer -> DrawRect(glm::vec2(4, 4), glm::vec2(portrait + border), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 6), glm::vec2(width + 4, 34), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 40), glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawRect(glm::vec2(border + border / 2, portrait + (border + border / 2)), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));

        // bar background
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 10), glm::vec2(width, 12), 0, HEXtoRGB(0x191919));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 24), glm::vec2(width, 12), 0, HEXtoRGB(0x191919));

        // portrait
        game -> renderer -> DrawRect(glm::vec2(border), glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFF));

        // health
        float health = game -> getCurrentWorld() -> player -> getHealth() / game -> getCurrentWorld() -> player -> getMaxHealth();
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 10), glm::vec2(width * health, 12), 0, HEXtoRGB(0xC04C47));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 20), glm::vec2(width * health, 2), 0, HEXtoRGB(0x963531));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2) + width * health - 2, 10), glm::vec2(2, 12), 0, HEXtoRGB(0x963531));

        // shield
        float shield = game -> getCurrentWorld() -> player -> getShield() / game -> getCurrentWorld() -> player -> getMaxShield();
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 24), glm::vec2(width * shield, 12), 0, HEXtoRGB(0x6699AA));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 34), glm::vec2(width * shield, 2), 0, HEXtoRGB(0x426977));
        game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2) + width * shield - 2, 24), glm::vec2(2, 12), 0, HEXtoRGB(0x426977));

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
    std::ostringstream healthStream, shieldStream, coinStream, damageStream;
    healthStream << std::fixed << std::setprecision(1) << game -> getCurrentWorld() -> player -> getHealth() << " / " << game -> getCurrentWorld() -> player -> getMaxHealth();
    shieldStream << std::fixed << std::setprecision(1) << game -> getCurrentWorld() -> player -> getShield() << " / " << game -> getCurrentWorld() -> player -> getMaxShield();
    damageStream << game -> getCurrentWorld() -> player -> getDamage();

    coinStream << "$" << game -> getCurrentWorld() -> player -> getCoins();

    auto textWidth = std::max(
        std::max(
            game -> text -> GetWidth(healthStream.str(), 16),
            game -> text -> GetWidth(shieldStream.str(), 16)),

        game -> text -> GetWidth(damageStream.str(), 16)
    );

    int coinWidth = game -> text -> GetWidth(coinStream.str(), 16.0f) + border / 2 + 2;

    // background shadows
    game -> renderer -> DrawRect(glm::vec2(4, 4) + glm::vec2(2), glm::vec2(portrait + border), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 6) + glm::vec2(2), glm::vec2(textWidth + (2 * border), 63), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 69) + glm::vec2(2), glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawRect(glm::vec2(border + border / 2, portrait + (border + border / 2)) + glm::vec2(2), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

    // backgrounds
    game -> renderer -> DrawRect(glm::vec2(4, 4), glm::vec2(portrait + border), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 6), glm::vec2(textWidth + (2 * border), 63), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawRect(glm::vec2(portrait + (border + border / 2), 69), glm::vec2(coinWidth, 16 + border / 2), 0, HEXtoRGB(0x2F2F2F));
    game -> renderer -> DrawRect(glm::vec2(border + border / 2, portrait + (border + border / 2)), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));

    // portrait
    game -> renderer -> DrawRect(glm::vec2(border), glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFF));

    // stats
    glm::vec2 startPos = glm::vec2(portrait + (border + border / 2), border + 16);
    game -> renderer -> DrawText(healthStream.str(), startPos + glm::vec2(border, 0 * 20), 16.0f, true, HEXtoRGB(0xC04C47));
    game -> renderer -> DrawText(shieldStream.str(), startPos + glm::vec2(border, 1 * 20), 16.0f, true, HEXtoRGB(0x6699AA));
    game -> renderer -> DrawText(damageStream.str(), startPos + glm::vec2(border, 2 * 20), 16.0f, true, HEXtoRGB(0xFFFFFF));

    // coins
    game -> renderer -> DrawText(coinStream.str(), glm::vec2(portrait + (2 * border), border + 77), 16.0f, true, HEXtoRGB(0xFDFF74));
}

void InGame::renderPlayerShop() {
    glm::vec2 pos;
    glm::vec2 size(2 * portrait + border + width, 6 * portrait);
    isAdvancedView ? pos = glm::vec2(4, 2 * (portrait + border) - 19) : pos = glm::vec2(4, 2 * border + portrait + portrait / 2);

    // background shadows
    game -> renderer -> DrawRect(pos + glm::vec2(2), size, 0, HEXtoRGB(0x000000));

    // backgrounds
    game -> renderer -> DrawRect(pos, size, 0, HEXtoRGB(0x2F2F2F));

    // upgrade panels
    for (auto &panel : upgradePanels) {
        panel -> render();
    }
}

void InGame::renderTurretShop() {
    auto screenWidth = game -> getSize().x;
    glm::vec2 size(2 * portrait + border + width, 6 * portrait);

    if (isTurretShopOpen) {
        // background shadows
        game -> renderer -> DrawRect(glm::vec2(screenWidth - size.x - 4, portrait) + glm::vec2(2), size, 0, HEXtoRGB(0x000000));
        game -> renderer -> DrawRect(glm::vec2(screenWidth - size.x - 4 - portrait / 2, portrait) + glm::vec2(2), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

        // backgrounds
        game -> renderer -> DrawRect(glm::vec2(screenWidth - size.x - 4, portrait), size, 0, HEXtoRGB(0x2F2F2F));
        game -> renderer -> DrawRect(glm::vec2(screenWidth - size.x - 4 - portrait / 2, portrait), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));

        // turret image
        game -> renderer -> DrawRect(glm::vec2(screenWidth - size.x, portrait + 4) + glm::vec2(4, 4), glm::vec2(portrait), 0, HEXtoRGB(0xFFFFFFF));

        for (auto &panel : shopPanels) {
            panel -> render();
        }
    } else {
        // background shadows
        game -> renderer -> DrawRect(glm::vec2(screenWidth - 2 - portrait / 2, portrait) + glm::vec2(2), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x000000));

        // backgrounds
        game -> renderer -> DrawRect(glm::vec2(screenWidth - 2 - portrait / 2, portrait), glm::vec2(portrait / 2, portrait / 2), 0, HEXtoRGB(0x2F2F2F));
    }
}

void InGame::renderTurretUpgrades() {
    auto turret = game -> getCurrentWorld() -> turret -> getSelectedTurret();
    if (!turret && !game -> getCurrentWorld() -> turret -> isUpgrading()) return;

    auto popupPosition = game -> getCurrentWorld() -> turret -> menuPosition;
    auto popupSize = game -> getCurrentWorld() -> turret -> menuSize;

    // shadow
    game -> renderer -> DrawRect(popupPosition + glm::vec2(2), popupSize, 0.0f, HEXtoRGB(0x000000));

    // background
    game -> renderer -> DrawRect(popupPosition, popupSize, 0.0f, HEXtoRGB(0x2F2F2F));

    // X button
    turretUpgradeClose.render();
    game -> renderer -> DrawText("X", popupPosition + glm::vec2(popupSize.x - (border + game -> text -> GetWidth("X", 16) + 1), border + 14), 16.0f, true, HEXtoRGB(0xFF0000));

    // Skill tree
    auto skillTree = game -> getCurrentWorld() -> turret -> getUpgradeManager().getSkillTree(game -> getCurrentWorld() -> turret -> getSelectedTurret() -> getType());
    auto nodePositions = game -> getCurrentWorld() -> turret -> calculateNodePositions();

    for (const auto& [upgrade, prereqs] : skillTree) {
        glm::vec2 nodePosition = nodePositions[upgrade];

        Button upgradeButton(nodePosition, glm::vec2(24), "", HEXtoRGB(0x4F4F4F));
        upgradeButton.render();

        if (upgradeButton.isClicked()) {
            if (game -> getCurrentWorld() -> turret -> unlockTurretUpgrade(upgrade -> getName())) {
                std::cout << "Upgrade unlocked: " << upgrade -> getName() << std::endl;
            } else {
                std::cout << "Cannot unlock upgrade: " << upgrade -> getName() << std::endl;
            }
        }

        for (auto prereq : prereqs) {
            glm::vec2 prereqPosition = nodePositions[prereq];
            renderConnection(nodePosition + glm::vec2(12), prereqPosition + glm::vec2(12), HEXtoRGB(0xFFFFFF));
        }
    }
}

void InGame::renderWaveInfo() {
    if (!game -> getCurrentWorld()) return;

    std::string waveText = "Wave: " + std::to_string(game -> getCurrentWorld() -> wave -> getCurrentWaveIndex());

    int enemiesAlive = 0;
    if (game -> getCurrentWorld() -> wave -> getCurrentEnemies()) {
        enemiesAlive = std::count_if(game -> getCurrentWorld() -> wave -> getCurrentEnemies() -> begin(),
                                   game -> getCurrentWorld() -> wave -> getCurrentEnemies() -> end(),
                                   [](const Enemy& e) { return e.getHealth() > 0; });
    }

    std::string enemiesText = "Enemies: " + std::to_string(enemiesAlive);

    int waveWidth = game -> text -> GetWidth(waveText, 16.0f);
    int enemiesWidth = game -> text -> GetWidth(enemiesText, 16.0f);
    int totalWidth = std::max(waveWidth, enemiesWidth) + 24;

    glm::vec2 panelPos(game -> getSize().x / 2 - totalWidth / 2, 6);

    game -> renderer -> DrawRect(panelPos + glm::vec2(2), glm::vec2(totalWidth, 44), 0, HEXtoRGB(0x000000)); // shadow
    game -> renderer -> DrawRect(panelPos, glm::vec2(totalWidth, 44), 0, HEXtoRGB(0x2F2F2F)); // background

    game -> renderer -> DrawText(waveText, panelPos + glm::vec2(totalWidth / 2 - waveWidth / 2, 18), 16.0f, true);
    game -> renderer -> DrawText(enemiesText, panelPos + glm::vec2(totalWidth / 2 - enemiesWidth / 2, 38), 16.0f, true);
}

void InGame::renderScore() {
    game -> renderer -> DrawRect(glm::vec2(border / 2, game -> getSize().y - 32.0f) + glm::vec2(2), glm::vec2(game -> text -> GetWidth("Score: " + std::to_string(game -> getCurrentWorld() -> getScore()), 16.0f) + 8, 26), 0, HEXtoRGB(0x000000));

    game -> renderer -> DrawRect(glm::vec2(border / 2, game -> getSize().y - 32.0f), glm::vec2(game -> text -> GetWidth("Score: " + std::to_string(game -> getCurrentWorld() -> getScore()), 16.0f) + 8, 26), 0, HEXtoRGB(0x2F2F2F));

    game -> renderer -> DrawText("Score: " + std::to_string(game -> getCurrentWorld() -> getScore()), glm::vec2(border, game -> getSize().y - 12.0f), 16.0f, true);
}

void InGame::renderEnemyPopups() {
    std::string info = hoveredEnemy -> getInfoString();
    glm::vec2 popupPos = hoveredEnemy -> getPosition() + glm::vec2(32 * hoveredEnemy -> getSizeScale() + 16, -16);

    int lineCount = std::count(info.begin(), info.end(), '\n') + 1;
    int textHeight = lineCount * 24;
    int maxLineWidth = 0;

    std::istringstream iss(info);
    std::string line;
    while (std::getline(iss, line)) {
        int lineWidth = game -> text -> GetWidth(line, 24.0f);
        if (lineWidth > maxLineWidth) maxLineWidth = lineWidth;
    }

    game -> renderer -> SetProjection(game -> camera -> getCameraProjection());
    game -> renderer -> DrawRect(popupPos + glm::vec2(2), glm::vec2(maxLineWidth + 20, textHeight + 28), 0, HEXtoRGB(0x000000)); // shadow
    game -> renderer -> DrawRect(popupPos, glm::vec2(maxLineWidth + 20, textHeight + 28), 0, HEXtoRGB(0x2F2F2F)); // background

    iss.clear();
    iss.str(info);
    int lineIndex = 1;
    while (std::getline(iss, line)) {
        game -> renderer -> DrawText(line, popupPos + glm::vec2(10, 10 + lineIndex * 24), 24.0f, true, HEXtoRGB(0xFFFFFF));
        lineIndex++;
    }
    game -> renderer -> SetProjection(game -> camera -> getStaticProjection());
}

void InGame::renderPopup(const std::string &text, const glm::vec2 &position, const glm::vec3 &color) {
    int textWidth = game -> text -> GetWidth(text, 16.0f);
    int textHeight = 16;

    game -> renderer -> DrawRect(position + glm::vec2(2), glm::vec2(textWidth + 10, textHeight + 10), 0, HEXtoRGB(0x000000)); // shadow
    game -> renderer -> DrawRect(position, glm::vec2(textWidth + 10, textHeight + 10), 0, HEXtoRGB(0x2F2F2F));

    game -> renderer -> DrawText(text, position + glm::vec2(5, textHeight + 2), 16.0f, true, color);
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