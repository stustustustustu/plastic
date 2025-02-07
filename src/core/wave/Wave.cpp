#include "Wave.h"

Wave::Wave() {}

Wave::Wave(int n, GLFWwindow* window) {
    Enemy::populate(enemies, n, window, 30);
}

void Wave::addEnemy(Enemy& enemy) {
    enemies.push_back(enemy);
}

std::vector<Enemy> &Wave::getEnemies() {
    return enemies;
}