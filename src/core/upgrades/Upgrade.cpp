#include "Upgrade.h"

Upgrade::Upgrade(const std::string &name, int cost, UpgradeType type, float multiplier, const std::string &description, bool mutuallyExclusive) : name(name), cost(cost), type(type), multiplier(multiplier), description(description), mutuallyExclusive(mutuallyExclusive) {}

const std::string & Upgrade::getName() const {
    return this -> name;
}

void Upgrade::setName(const std::string &name) {
    this -> name = name;
}

int Upgrade::getCost() const {
    return this -> cost;
}

void Upgrade::setCost(int cost) {
    this -> cost = cost;
}

float Upgrade::getMultiplier() const {
    return this -> multiplier;
}

void Upgrade::setMultiplier(float multiplier) {
    this -> multiplier = multiplier;
}

UpgradeType Upgrade::getType() const {
    return this -> type;
}

void Upgrade::setType(UpgradeType type) {
    this -> type = type;
}

const std::string & Upgrade::getDescription() const {
    return this -> description;
}

void Upgrade::setDescription(const std::string &description) {
    this -> description = description;
}

bool Upgrade::isUnlocked() const {
    return this -> unlocked;
}

void Upgrade::setUnlocked(bool unlocked) {
    this -> unlocked = unlocked;
}

bool Upgrade::isTurretUpgrade() const {
    return (type == TURRET_DAMAGE || type == TURRET_FIRERATE || type == TURRET_RANGE || type == TURRET_SPECIAL);
}

bool Upgrade::isMutuallyExclusive() const {
    return this -> mutuallyExclusive;
}
