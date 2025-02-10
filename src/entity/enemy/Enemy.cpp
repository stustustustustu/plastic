#include "Enemy.h"
#include "../src/Game.h"

Enemy::Enemy(EnemyType type, const std::vector<float>& position): Entity(position), type(type) {
    auto[weight, health, damage, speed] = getEnemyData().at(type);
}

std::vector<Enemy> Enemy::generateEnemies(int index, int totalWeight) {
    auto window = Game::getInstance() -> window;

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    const int halfWidth = 16;
    const int halfHeight = 16;
    const int border = 4;
    const float minDistance = 50;

    std::vector<Enemy> enemies;
    auto enemyData = getEnemyData();

    std::vector<EnemyType> validTypes;

    for (const auto &[type, data] : enemyData) {
        if (std::get<0>(data) <= totalWeight) {
            validTypes.push_back(type);
        }
    }

    std::mt19937 rng(std::random_device{}()); // source: www.stackoverflow.com -> someone said its better than rand() for this case
    std::uniform_real_distribution<> dist(0.0, 1.0);

    while (totalWeight > 0 && !validTypes.empty()) {
        std::shuffle(validTypes.begin(), validTypes.end(), rng);
        EnemyType type = validTypes[validTypes.size() / 2];
        auto [weight, health, damage, speed] = enemyData.at(type);

        if (weight > totalWeight) continue;

        float x, y;

        do {
            x = rand() % (screenWidth - 2 * (halfWidth + border)) + border;
            y = rand() % (screenHeight - 2 * (halfHeight + border)) + border;
        } while (!isPositionValidWithOthers(x, y, halfWidth, halfHeight, border, screenWidth, screenHeight, enemies, minDistance));

        enemies.emplace_back(type, std::vector<float>{x, y});
        totalWeight -= weight;
    }

    return enemies;
}

std::map<EnemyType, std::tuple<float, float, float, float>> Enemy::getEnemyData() {
    return {
        {EnemyType::ABANDONED_SHIP, {100, 200, 50, 0.5f}},
        {EnemyType::SHIPPING_CONTAINER, {75, 150, 40, 0.6f}},
        {EnemyType::OIL_BARREL, {50, 100, 30, 0.7f}},
        {EnemyType::BUOY_SYSTEM, {30, 80, 20, 0.8f}},
        {EnemyType::PLASTIC_CANOE, {20, 60, 15, 0.9f}},
        {EnemyType::INDUSTRIAL_PACKAGING, {15, 50, 10, 1.0f}},
        {EnemyType::FISHERMANS_BARREL, {10, 40, 5, 1.1f}},
        {EnemyType::FLOATING_PLATFORM, {8, 30, 3, 1.2f}},
        {EnemyType::WATER_TANK, {6, 20, 2, 1.3f}},
        {EnemyType::FISHING_GEAR, {4, 15, 1, 1.4f}},
        {EnemyType::PLASTIC_BAG, {2, 10, 0.5f, 1.5f}},
        {EnemyType::TIRE, {1, 5, 0.2f, 1.6f}},
        {EnemyType::PLASTIC_BOTTLE, {0.5f, 3, 0.1f, 1.7f}}
    };
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

void Enemy::moveTowards(Entity& player) {
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

float Enemy::calculateDistance(const std::vector<float>& pos1, const std::vector<float>& pos2) {
    return std::sqrt(std::pow(pos1[0] - pos2[0], 2) + std::pow(pos1[1] - pos2[1], 2));
}