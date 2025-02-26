#include "UpgradeManager.h"

UpgradeManager::UpgradeManager(Inventory &inventory) : inventory(inventory) {
    addPath(DAMAGE);
    addPath(HEALTH);
    addPath(SPEED);
    addPath(SHIELD);
}

std::map<UpgradeType, UpgradePath> UpgradeManager::getPaths() const {
    return this -> paths;
}

void UpgradeManager::addPath(UpgradeType type) {
    if (paths.find(type) != paths.end()) {
        throw std::invalid_argument("Upgrade path for this type already exists.");
    }
    paths.emplace(type, UpgradePath(type));
}

bool UpgradeManager::upgrade(UpgradeType type) {
    auto it = paths.find(type);
    if (it == paths.end()) {
        std::cout << "Upgrade path for type " << type << " not found." << std::endl;
        return false;
    }

    UpgradePath& path = it -> second;

    const Upgrade& nextUpgrade = path.getNextUpgrade();
    int cost = nextUpgrade.getCost();

    if (inventory.hasEnoughCoins(cost)) {
        if (inventory.spendCoins(cost)) {
            path.upgrade();
            return true;
        }
    } else {
        std::cout << "Not enough coins to upgrade " << nextUpgrade.getName() << "." << std::endl;
    }
    return false;
}

float UpgradeManager::getMultiplier(UpgradeType type) const {
    auto it = paths.find(type);
    if (it != paths.end()) {
        return it -> second.getCurrentUpgrade().getMultiplier();
    }

    return 1.0f;
}

int UpgradeManager::getCurrentLevel(UpgradeType type) const {
    auto it = paths.find(type);
    if (it != paths.end()) {
        return it -> second.getLevel();
    }
    return 0;
}

int UpgradeManager::getCost(UpgradeType type, int level) const {
    auto it = paths.find(type);
    if (it != paths.end()) {
        UpgradePath path = it -> second;
        return path.getNextUpgrade().getCost();

    }
    return 0;
}

bool UpgradeManager::hasUpgradePath(UpgradeType type) const {
    return paths.find(type) != paths.end();
}