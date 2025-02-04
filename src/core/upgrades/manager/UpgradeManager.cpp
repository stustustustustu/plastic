#include "UpgradeManager.h"

UpgradeManager::UpgradeManager(Inventory &inventory) : inventory(inventory) {}

void UpgradeManager::Init() {
    // populate paths
}

void UpgradeManager::addPath(const std::string &name, const UpgradePath &path) {
    if (paths.find(name) != paths.end()) {
        throw std::invalid_argument("Upgrade path with this name already exists: " + name);
    }
    paths.emplace(name, path);
}

bool UpgradeManager::upgrade(const std::string &name) {
    auto it = paths.find(name);
    if (it == paths.end()) {
        std::cout << "Upgrade path " << name << " not found." << std::endl;
        return false;
    }

    UpgradePath& path = it -> second;
    if (!path.canUpgrade()) {
        std::cout << "Already at max level for " << name << "." << std::endl;
        return false;
    }

    const Upgrade& nextUpgrade = path.getNextUpgrade();
    int cost = nextUpgrade.getCost();

    if (inventory.hasEnoughCoins(cost)) {
        if (inventory.spendCoins(cost)) {
            path.upgrade();
            std::cout << "Upgraded " << name << " to level " << getCurrentLevel(name) << "." << std::endl;
            return true;
        }
    } else {
        std::cout << "Not enough coins to upgrade " << name << "." << std::endl;
    }
    return false;
}

float UpgradeManager::getMultiplier(const std::string &name) const {
    auto it = paths.find(name);
    if (it != paths.end()) {
        return it -> second.getCurrentUpgrade().getMultiplier();
    }

    return 1.0f;
}

int UpgradeManager::getCurrentLevel(const std::string &name) const {
    auto it = paths.find(name);
    if (it != paths.end()) {
        return it -> second.getLevel();
    }

    return 0;
}

int UpgradeManager::getCost(const std::string &name, int level) const {
    auto it = paths.find(name);
    if (it != paths.end()) {
        const UpgradePath& path = it -> second;
        if (path.canUpgrade()) {
            return path.getNextUpgrade().getCost();
        }
    }

    return 0;
}

bool UpgradeManager::hasUpgradePath(const std::string &name) const {
    return paths.find(name) != paths.end();
}
