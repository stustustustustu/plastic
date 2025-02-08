#include "Wave.h"

Wave::Wave(int index, int weight, bool bossfight) : index(index), weight(weight), bossfight(bossfight) {}

void Wave::addEnemies(const std::vector<Enemy> &enemies) {
    this -> enemies.insert(this -> enemies.end(), enemies.begin(), enemies.end());
}

std::vector<Enemy> & Wave::getEnemies() {
    return this -> enemies;
}

int Wave::getIndex() const {
    return this -> index;
}

void Wave::setIndex(int index) {
    this -> index = index;
}

int Wave::getWeight() const {
    return this -> weight;
}

void Wave::setWeight(int weight) {
    this -> weight = weight;
}

bool Wave::isBossfight() const {
    return this -> bossfight;
}

void Wave::setBossfight(bool bossfight) {
    this -> bossfight = bossfight;
}
