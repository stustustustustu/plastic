#include "TurretUpgradeManager.h"

TurretUpgradeManager::TurretUpgradeManager() {}

TurretUpgradeManager::~TurretUpgradeManager() {
    for (auto upgrade : upgrades) {
        delete upgrade;
    }
}

void TurretUpgradeManager::addUpgrade(Upgrade* upgrade, const std::vector<Upgrade*>& prerequisites) {
    upgrades.push_back(upgrade);
    this->prerequisites[upgrade] = prerequisites;
}

const std::vector<Upgrade*>& TurretUpgradeManager::getAvailableUpgrades() const {
    return upgrades;
}

bool TurretUpgradeManager::unlockUpgrade(Turret& turret, const std::string& upgradeName) {
    for (auto upgrade : upgrades) {
        if (upgrade->getName() == upgradeName) {
            bool canUnlock = true;
            for (auto prereq : prerequisites[upgrade]) {
                if (!prereq->isUnlocked()) {
                    canUnlock = false;
                    break;
                }
            }

            if (canUnlock) {
                upgrade->setUnlocked(true);
                applyUpgradeEffect(turret, upgrade);
                return true;
            }
            break;
        }
    }
    return false;
}

void TurretUpgradeManager::applyUpgradeEffect(Turret& turret, Upgrade* upgrade) {
    switch (upgrade -> getType()) {
        case TURRET_DAMAGE:
            turret.setDamage(turret.getDamage() * upgrade -> getMultiplier());
            break;
        case TURRET_FIRERATE:

            break;
        case TURRET_RANGE:

            break;
        case TURRET_SPECIAL:

            break;
        default:
            break;
    }
}

std::map<Upgrade*, std::vector<Upgrade*>> TurretUpgradeManager::getSkillTree() const {
    return prerequisites;
}