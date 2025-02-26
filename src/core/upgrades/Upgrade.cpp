#include "Upgrade.h"

Upgrade::Upgrade(const std::string &name, int cost, UpgradeType type, float multiplier, const std::string &description) : name(name), cost(cost), type(type), multiplier(multiplier), description(description) {}

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
