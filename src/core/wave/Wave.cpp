#include "Wave.h"

Wave::Wave() {}

Wave::Wave(int n, GLFWwindow* window) {
    Entity::populate(enemies, n, window);
}

void Wave::addEnemy(Entity& enemy) {
    enemies.push_back(enemy);
}

std::vector<Entity> &Wave::getEnemies() {
    return enemies;
}