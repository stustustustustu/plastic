#include "../Entity.h"
#include "../../config.h"

float calculateDistance(const std::vector<float>& pos1, const std::vector<float>& pos2) {
    return std::sqrt(std::pow(pos1[0] - pos2[0], 2) + std::pow(pos1[1] - pos2[1], 2));
}

void moveTowards(Entity& enemy, const std::vector<float>& targetPos) {
    auto currentPos = enemy.getPosition();

    // Calculate the direction vector
    std::vector<float> direction = {
        targetPos[0] - currentPos[0],
        targetPos[1] - currentPos[1]
    };

    float magnitude = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
    if (magnitude > 0) {
        // Normalize direction
        direction[0] /= magnitude;
        direction[1] /= magnitude;

        // Scale by speed
        std::vector<float> delta = {
            direction[0],
            direction[1]
        };

        // Apply delta to the enemy's position
        enemy.move(delta);
    }
}

bool isPositionValid(float x, float y, int halfWidth, int halfHeight, int border, int screenWidth, int screenHeight) {
    return x - halfWidth >= border &&
           x + halfWidth <= screenWidth - border &&
           y - halfHeight >= border &&
           y + halfHeight <= screenHeight - border;
}

bool isPositionValidWithOthers(float x, float y, int halfWidth, int halfHeight, int border, int screenWidth, int screenHeight, const std::vector<Entity>& enemies, float minDistance) {
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

void Entity::populate(std::vector<Entity>& enemies, int n, GLFWwindow* window) {
    srand(time(NULL));

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    const int halfWidth = 16;
    const int halfHeight = 16;
    const int border = 4;

    for (int i = 0; i < n; ++i) {
        float x, y;
        do {
            x = border + rand() % (screenWidth - 2 * (halfWidth + border));
            y = border + rand() % (screenHeight - 2 * (halfHeight + border));
        } while (!isPositionValidWithOthers(x, y, halfWidth, halfHeight, border, screenWidth, screenHeight, enemies, 48.0f));

        enemies.emplace_back(std::vector<float>{x, y});
    }
}

void Entity::moveTo(Entity& player) {
    std::vector<float> targetPos = player.getPosition();

    // Move the enemy towards the player's position
    moveTowards(*this, targetPos);
}