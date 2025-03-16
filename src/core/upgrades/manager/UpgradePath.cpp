#include "UpgradePath.h"

UpgradePath::UpgradePath(UpgradeType type) : type(type) {
    generateNextUpgrade();
}

UpgradeType UpgradePath::getType() const {
    return this -> type;
}

const std::vector<Upgrade> & UpgradePath::getUpgrades() const {
    return this -> upgrades;
}

const Upgrade &UpgradePath::getCurrentUpgrade() const {
    return this -> upgrades.at(getLevel());
}

const Upgrade &UpgradePath::getNextUpgrade() {
    if (getLevel() >= upgrades.size() - 1) {
        return upgrades.back();
    }
    return this -> upgrades.at(getLevel() + 1);
}

void UpgradePath::upgrade() {
    level++;
    generateNextUpgrade();
}

int UpgradePath::getLevel() const {
    return this -> level;
}

void UpgradePath::setLevel(int level) {
    if (level >= 0 && level < upgrades.size()) {
        this -> level = level;
    }
}

void UpgradePath::restoreLevel(int level) {
    for (int i = 0; i < level; ++i) {
        upgrade();
    }
}

std::string UpgradePath::generateName() const {
    std::string levelStr = intToRoman(level + 1);

    switch (type) {
        case DAMAGE:
            return "DAMAGE " + levelStr;
        case HEALTH:
            return "HEALTH " + levelStr;
        case SPEED:
            return "SPEED " + levelStr;
        case SHIELD:
            return "SHIELD " + levelStr;
        default:
            return "UPGRADE " + levelStr;
    }
}

int UpgradePath::generateCost() const {
    return static_cast<int>(30 * std::pow(1.5, level + 1)); // $50 base cost
}

float UpgradePath::generateMultiplier() const {
    bool isSpecialUpgrade = (rand() % 10 == 0);

    if (isSpecialUpgrade) {
        return 1.15f + static_cast<float>(rand() % 16) * 0.01f; // 15% to 30%
    } else {
        return 1.10f + static_cast<float>(rand() % 6) * 0.01f; // 10% to 15%
    }
}

std::string UpgradePath::generateDescription(float multiplier) const {
    std::ostringstream description;

    description << "Increases ";

    switch (type) {
        case DAMAGE: description << "damage"; break;
        case HEALTH: description << "health"; break;
        case SPEED: description << "speed"; break;
        case SHIELD: description << "shield"; break;
        default: description << "stat"; break;
    }

    int percentageIncrease = static_cast<int>((multiplier - 1.0f) * 100.0f);

    description << " by " << percentageIncrease << "%.";

    return description.str();
}


Upgrade UpgradePath::generateNextUpgrade() {
    std::string name = generateName();
    int cost = generateCost();
    float multiplier = generateMultiplier();
    std::string description = generateDescription(multiplier);

    Upgrade upgrade(name, cost, type, multiplier, description);
    upgrades.push_back(upgrade);
    return upgrade;
}
