#include "Enemy.h"
#include "../src/Game.h"

auto const game = Game::getInstance();

Enemy::Enemy(EnemyType type, const std::vector<float>& position, float health, float damage, float speed) : Entity(position, speed * 0.5f, damage, health), type(type) {
    setCoins(std::max(5, rand() % static_cast<int>(health) / 10));

    switch (game -> getDifficulty()) {
        case EASY:
            setHealth(health * 0.8f);
            setDamage(damage * 0.8f);
            setSpeed(getSpeed() * 0.9f);
            setCoins(std::max(1, static_cast<int>(getCoins() * 1.2f)));
            break;
        case MEDIUM:
            setHealth(health * 1.0f);
            setDamage(damage * 1.0f);
            setSpeed(getSpeed() * 1.0f);
            setCoins(std::max(1, static_cast<int>(getCoins() * 1.0f)));
            break;
        case HARD:
            setHealth(health * 1.2f);
            setDamage(damage * 1.2f);
            setSpeed(getSpeed() * 1.1f);
            setCoins(std::max(1, static_cast<int>(getCoins() * 0.8f)));
            break;
        case EXPERT:
            setHealth(health * 1.4f);
            setDamage(damage * 1.4f);
            setSpeed(getSpeed() * 1.15f);
            setCoins(std::max(1, static_cast<int>(getCoins() * 0.6f)));
            break;
        case IMPOSSIBLE:
            setHealth(health * 1.6f);
            setDamage(damage * 1.6f);
            setSpeed(getSpeed() * 1.2f);
            setCoins(std::max(1, static_cast<int>(getCoins() * 0.4f)));
            break;
    }
}

std::vector<Enemy> Enemy::generateEnemies(int index, int totalWeight) {
    auto window = Game::getInstance()->window;
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    const float minDist = 150;

    std::vector<Enemy> enemies;
    auto enemyData = getEnemyData();
    std::vector<EnemyType> validTypes;

    // Filter enemy types based on weight
    for (const auto &[type, data] : enemyData) {
        if (std::get<0>(data) <= totalWeight) {
            validTypes.push_back(type);
        }
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    while (totalWeight > 0 && !validTypes.empty()) {
        std::shuffle(validTypes.begin(), validTypes.end(), rng);
        EnemyType type = validTypes[validTypes.size() / 2];
        auto [weight, health, damage, speed] = enemyData.at(type);

        if (weight > totalWeight) continue;

        float x, y;
        do {
            x = screenWidth / 2 + (rand() % (screenWidth * 2)) - screenWidth;
            y = screenHeight / 2 + (rand() % (screenHeight * 2)) - screenHeight;

        } while (game -> generator -> distanceToNearestLand(x, y) < minDist);

        enemies.emplace_back(type, std::vector<float>{x, y}, health, damage, speed);
        totalWeight -= weight;
    }

    return enemies;
}

std::map<EnemyType, std::tuple<float, float, float, float>> Enemy::getEnemyData() {
    return {
        {EnemyType::ABANDONED_SHIP, {100, 200, 50, 0.4f}},      // Slow but tanky
        {EnemyType::SHIPPING_CONTAINER, {75, 150, 40, 0.5f}},   // Slightly faster, less health
        {EnemyType::OIL_BARREL, {50, 100, 30, 0.6f}},           // Balanced health and speed
        {EnemyType::BUOY_SYSTEM, {30, 80, 20, 0.7f}},           // Faster, lower health
        {EnemyType::PLASTIC_CANOE, {20, 60, 15, 0.8f}},         // Even faster, fragile
        {EnemyType::INDUSTRIAL_PACKAGING, {15, 50, 10, 0.9f}},  // Close to player speed, low health
        {EnemyType::FISHERMANS_BARREL, {10, 40, 5, 1.0f}},      // Matches player speed, very low health
        {EnemyType::FLOATING_PLATFORM, {8, 30, 3, 1.1f}},       // Slightly faster than player, fragile
        {EnemyType::WATER_TANK, {6, 20, 2, 1.2f}},              // Fast, very low health
        {EnemyType::FISHING_GEAR, {4, 15, 1, 1.3f}},            // Very fast, extremely fragile
        {EnemyType::PLASTIC_BAG, {2, 10, 0.5f, 1.4f}},          // Extremely fast, almost no health
        {EnemyType::TIRE, {1, 5, 0.2f, 1.5f}},                  // Fastest, almost no health
        {EnemyType::PLASTIC_BOTTLE, {0.5f, 3, 0.1f, 1.6f}}      // Fastest, almost no health
    };
}

void Enemy::moveTowards(const std::vector<float>& targetPos) {
    auto currentPos = this->getPosition();
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

        this->move(delta);
    }

    updateBounds();
}

float Enemy::calculateDistance(const std::vector<float>& pos1, const std::vector<float>& pos2) {
    return std::sqrt(std::pow(pos1[0] - pos2[0], 2) + std::pow(pos1[1] - pos2[1], 2));
}