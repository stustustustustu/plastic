#include "TurretManager.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void TurretManager::update() {
    for (auto &turret : turrets) {
        turret -> findTarget();
        turret -> rotateTowardsTarget();
        turret -> shoot();
    }
}

void TurretManager::render() {
    for (const auto &turret : turrets) {
        turret -> render(game -> texture);
    }

    if (showShop) {
        renderTurretShop();
    }

    if (showUpgrades && selectedTurret) {
        renderUpgradeMenu();
    }
}

void TurretManager::placeTurret(TurretType type, const std::vector<float> &position) {
    int cost = 25; // temp
    if (game -> inventory -> hasEnoughCoins(cost)) {
        if (game -> inventory -> spendCoins(cost)) {
            auto turret = std::make_shared<Turret>(position, type);
            turrets.push_back(turret);
            std::cout <<"Turret placed at [" << position[0] << ", " << position[1] << "]" << std::endl;
        }
    } else {
        std::cout << "Not enough coins to place turret." << std::endl;
    }
}

void TurretManager::openUpgradeMenu(std::shared_ptr<Turret> turret) {
    selectedTurret = turret;
    showUpgrades = true;
}

void TurretManager::renderTurretShop() {
    glm::vec2 shopPosition = {100, 100}; // Example position
    glm::vec2 shopSize = {200, 300};     // Example size
    game -> renderer -> DrawSpriteSheet(*game -> texture, shopPosition, 2, 32, 32, shopSize, 0, HEXtoRGB(0x323232));

    // Render buttons for each turret type
    glm::vec2 buttonSize = {150, 50};
    glm::vec2 buttonPosition = shopPosition + glm::vec2(25, 25);

    // Laser Turret Button
    game -> renderer -> DrawSpriteSheet(*game -> texture, buttonPosition, 2, 32, 32, buttonSize, 0, HEXtoRGB(0x646464));
    game -> renderer -> DrawText("Laser Turret", buttonPosition + glm::vec2(10, 10), 20.0f, 0, HEXtoRGB(0xFFFFFF));

    // Rifle Turret Button
    buttonPosition.y += 60;
    game -> renderer -> DrawSpriteSheet(*game -> texture, buttonPosition, 2, 32, 32, buttonSize, 0, HEXtoRGB(0x646464));
    game -> renderer -> DrawText("Rifle Turret", buttonPosition + glm::vec2(10, 10), 20.0f, 0,  HEXtoRGB(0xFFFFFF));

    // Bomb Turret Button
    buttonPosition.y += 60;
    game -> renderer -> DrawSpriteSheet(*game -> texture, buttonPosition, 2, 32, 32, buttonSize, 0, HEXtoRGB(0x646464));
    game -> renderer -> DrawText("Bomb Turret", buttonPosition + glm::vec2(10, 10), 20.0f, 0, HEXtoRGB(0xFFFFFF));
}

void TurretManager::renderUpgradeMenu() {
    glm::vec2 menuPosition = {300, 100};
    glm::vec2 menuSize = {200, 200};
    game -> renderer -> DrawSpriteSheet(*game -> texture, menuPosition, 2, 32, 32, menuSize, 0, HEXtoRGB(0x323232));

    // Render upgrade options
    glm::vec2 buttonSize = {150, 50};
    glm::vec2 buttonPosition = menuPosition + glm::vec2(25, 25);

    // Damage Upgrade Button
    game -> renderer -> DrawSpriteSheet(*game -> texture, buttonPosition, 2, 32, 32, buttonSize, 0, HEXtoRGB(0x646464));
    game -> renderer -> DrawText("Upgrade Damage", buttonPosition + glm::vec2(10, 10), 20.0f, 0, HEXtoRGB(0xFFFFFF));

    // Fire Rate Upgrade Button
    buttonPosition.y += 60;
    game -> renderer -> DrawSpriteSheet(*game -> texture, buttonPosition, 2, 32, 32, buttonSize, 0, HEXtoRGB(0x646464));
    game -> renderer -> DrawText("Upgrade Fire Rate", buttonPosition + glm::vec2(10, 10), 20.0f, 0, HEXtoRGB(0xFFFFFF));
}

void TurretManager::handleClick(const glm::vec2 &mousePos) {

}
