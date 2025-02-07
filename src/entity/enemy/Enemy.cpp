#include "Enemy.h"

#include "../Entity.h"

Enemy::Enemy(const std::vector<float>& position, float health, float speed) : Entity(position), health(health), speed(speed) {}

float Enemy::calculateDistance(const std::vector<float>& pos1, const std::vector<float>& pos2) {
    return std::sqrt(std::pow(pos1[0] - pos2[0], 2) + std::pow(pos1[1] - pos2[1], 2));
}

void Enemy::moveTowards(const std::vector<float>& targetPos) {
    auto currentPos = this -> getPosition();
    std::vector<float> direction = {
        targetPos[0] - currentPos[0],
        targetPos[1] - currentPos[1]
    };
    float magnitude = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
    if (magnitude > 0) {
        direction[0] /= magnitude;
        direction[1] /= magnitude;
        std::vector<float> delta = {
            direction[0] * getSpeed(),
            direction[1] * getSpeed()
        };
        this -> move(delta);
    }
}

void Enemy::moveTo(Entity& player) {
    std::vector<float> targetPos = player.getPosition();
    moveTowards(targetPos);
}

bool Enemy::isPositionValid(float x, float y, int halfWidth, int halfHeight, int border, int screenWidth, int screenHeight) {
    return x - halfWidth >= border &&
           x + halfWidth <= screenWidth - border &&
           y - halfHeight >= border &&
           y + halfHeight <= screenHeight - border;
}

bool Enemy::isPositionValidWithOthers(float x, float y, int halfWidth, int halfHeight, int border, int screenWidth, int screenHeight, const std::vector<Enemy>& enemies, float minDistance) {
    if (!isPositionValid(x, y, halfWidth, halfHeight, border, screenWidth, screenHeight)) {
        return false;
    }
    for (const auto& enemy : enemies) {
        float distance = calculateDistance({x, y}, enemy.getPosition());
        if (distance < minDistance) {
            return false;
        }
    }
    return true;
}

void Enemy::populate(std::vector<Enemy>& enemies, int count, GLFWwindow* window, float minDistance) {
    srand(static_cast<unsigned>(time(NULL)));
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    const int halfWidth = 16;
    const int halfHeight = 16;
    const int border = 4;

    for (int i = 0; i < count; ++i) {
        float x, y;
        do {
            x = border + rand() % (screenWidth - 2 * (halfWidth + border));
            y = border + rand() % (screenHeight - 2 * (halfHeight + border));
        } while (!isPositionValidWithOthers(x, y, halfWidth, halfHeight, border, screenWidth, screenHeight, enemies, minDistance));

        enemies.emplace_back(std::vector<float>{x, y}, 1.0f, 50);
    }
}