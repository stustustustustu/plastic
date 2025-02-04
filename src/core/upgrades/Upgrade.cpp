#include "Upgrade.h"

Upgrade::Upgrade(int cost, UpgradeType type, float multiplier, const std::string &description) : cost(cost), type(type), multiplier(multiplier), description(description) {}

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
