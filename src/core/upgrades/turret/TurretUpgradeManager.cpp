#include "TurretUpgradeManager.h"

TurretUpgradeManager::TurretUpgradeManager() {}

TurretUpgradeManager::~TurretUpgradeManager() {
    for (auto &tree : upgrades) {
        for (auto upgrade : tree.second) {
            delete upgrade;
        }
    }
}

void TurretUpgradeManager::addUpgrade(TurretType type, Upgrade* upgrade, const std::vector<Upgrade*>& prerequisites, const std::vector<Upgrade*>& mutuallyExclusive) {
    upgrades[type].push_back(upgrade);
    this -> prerequisites[upgrade] = prerequisites;

    if (!mutuallyExclusive.empty()) {
        for (auto exclusiveUpgrade : mutuallyExclusive) {
            mutuallyExclusiveGroups[upgrade].push_back(exclusiveUpgrade);
            mutuallyExclusiveGroups[exclusiveUpgrade].push_back(upgrade);
        }
    }
}

const std::vector<Upgrade*>& TurretUpgradeManager::getAvailableUpgrades(TurretType type) const {
    static const std::vector<Upgrade*> empty;
    auto it = upgrades.find(type);

    if (it != upgrades.end()) {
        return it -> second;
    }

    return empty;
}

bool TurretUpgradeManager::unlockUpgrade(Turret& turret, const std::string& upgradeName) {
    auto &upgradeList = upgrades[turret.getType()];
    for (auto upgrade : upgradeList) {
        if (upgrade -> getName() == upgradeName) {
            if (upgrade -> isUnlocked()) {
                return false;
            }

            bool canUnlock = true;
            for (auto prereq : prerequisites[upgrade]) {
                if (!prereq -> isUnlocked()) {
                    canUnlock = false;
                    break;
                }
            }

            if (canUnlock) {
                for (auto exclusiveUpgrade : mutuallyExclusiveGroups[upgrade]) {
                    if (exclusiveUpgrade -> isUnlocked()) {
                        canUnlock = false;
                        break;
                    }
                }
            }

            if (canUnlock) {
                upgrade -> setUnlocked(true);
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

std::map<Upgrade*, std::vector<Upgrade*>> TurretUpgradeManager::getSkillTree(TurretType type) const {
    std::map<Upgrade*, std::vector<Upgrade*>> skillTree;
    for (auto upgrade : upgrades.at(type)) {
        skillTree[upgrade] = prerequisites.at(upgrade);
    }
    return skillTree;
}