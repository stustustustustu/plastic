#include "UpgradePath.h"

UpgradePath::UpgradePath(std::string &name, const std::vector<Upgrade> &upgrades) : name(name), upgrades(upgrades) {}

const std::string & UpgradePath::getName() const {
    return this -> name;
}

void UpgradePath::setName(const std::string &name) {
    this -> name = name;
}

const std::vector<Upgrade> & UpgradePath::getUpgrades() const {
    return this -> upgrades;
}

const Upgrade &UpgradePath::getCurrentUpgrade() const {
    return this -> upgrades.at(getLevel());
}

const Upgrade &UpgradePath::getNextUpgrade() const {
    return this -> upgrades.at(getLevel() + 1);
}

bool UpgradePath::canUpgrade() const {
    return getLevel() < upgrades.size() - 1;
}

void UpgradePath::upgrade() {
    if (canUpgrade()) {
        level++;
    }
}

int UpgradePath::getLevel() const {
    return this -> level;
}

void UpgradePath::setLevel(int level) {
    if (level >= 0 && level < upgrades.size()) {
        this -> level = level;
    }
}
