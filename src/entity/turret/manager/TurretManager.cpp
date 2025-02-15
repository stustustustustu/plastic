#include "TurretManager.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void TurretManager::update() {
    for (auto &turret : turrets) {
        turret -> findTarget();
        turret -> rotateTowardsTarget();
        turret -> shoot();
    }

    shop -> update();
}

void TurretManager::render() {
    game -> renderer -> SetProjection(game -> camera -> getCameraProjection());

    for (const auto &turret : turrets) {
        turret -> render(game -> texture);
    }

    game -> renderer -> SetProjection(game -> camera -> getStaticProjection());

    if (showShop) {
        shop -> render();
    }

    if (showUpgrades && selectedTurret) {
        //renderUpgradeMenu();
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

void TurretManager::handleClick(const glm::vec2 &mousePos) {

}
