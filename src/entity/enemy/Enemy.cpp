#include "Enemy.h"
#include "../src/Game.h"

auto const game = Game::getInstance();

Enemy::Enemy(EnemyType type, const glm::vec2& position, float health, float damage, float speed)
    : Entity(position, speed * 0.5f, damage, health), type(type), spawn({position, std::chrono::milliseconds(0)}) {
    setCoins(std::max(5, rand() % static_cast<int>(health) / 10));

    if (game -> getCurrentWorld()) {
        switch (game -> getCurrentWorld() -> getDifficulty()) {
            case EASY:
                setHealth(health * 0.8f);
                setDamage(damage * 0.8f);
                setSpeed(getSpeed() * 0.8f);
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
                setSpeed(getSpeed() * 1.2f);
                setCoins(std::max(1, static_cast<int>(getCoins() * 0.8f)));
                break;
            case EXPERT:
                setHealth(health * 1.4f);
                setDamage(damage * 1.4f);
                setSpeed(getSpeed() * 1.4f);
                setCoins(std::max(1, static_cast<int>(getCoins() * 0.6f)));
                break;
            case IMPOSSIBLE:
                setHealth(health * 1.6f);
                setDamage(damage * 1.6f);
                setSpeed(getSpeed() * 1.6f);
                setCoins(std::max(1, static_cast<int>(getCoins() * 0.4f)));
                break;
        }
    }
}

std::vector<Enemy> Enemy::generateEnemies(int index, int totalWeight) {
    auto window = Game::getInstance() -> window;
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

        } while (game -> getCurrentWorld() -> island -> distanceToNearestLand(x, y) < minDist);

        enemies.emplace_back(type, glm::vec2(x, y), health, damage, speed);

        Event event;

        event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - game -> getCurrentWorld() -> replay -> getStartTime()
        );
        event.type = EventType::ENEMY_SPAWN;

        glm::vec2 pos = {x, y};
        event.data.resize(sizeof(EnemyType) + 2 * sizeof(glm::vec2) + sizeof(float));
        memcpy(event.data.data(), &type, sizeof(EnemyType));
        memcpy(event.data.data() + sizeof(EnemyType), &pos, sizeof(glm::vec2));
        memcpy(event.data.data() + sizeof(EnemyType) + 2 * sizeof(glm::vec2), &speed, sizeof(float));

        game -> getCurrentWorld() -> replay -> addEvent(event);

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

EnemyType Enemy::getType() const {
    return this -> type;
}

void Enemy::moveTowards(const glm::vec2& targetPos) {
    auto currentPos = this -> getPosition();
    glm::vec2 direction = targetPos - currentPos;

    float magnitude = glm::length(direction);
    if (magnitude > 0) {
        direction /= magnitude;

        glm::vec2 delta = direction * getSpeed();
        this -> move(delta);

        Event event;
        event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - game -> getCurrentWorld() -> replay -> getStartTime()
        );
        event.type = EventType::ENEMY_MOVE;

        glm::vec2 position = getPosition();
        event.data.resize(sizeof(EnemyType) + sizeof(glm::vec2));
        memcpy(event.data.data(), &type, sizeof(EnemyType));
        memcpy(event.data.data() + sizeof(EnemyType), &position, sizeof(glm::vec2));

        game -> getCurrentWorld() -> replay -> addEvent(event);
    }

    updateBounds();
}

float Enemy::calculateDistance(const glm::vec2& pos1, const glm::vec2& pos2) {
    return glm::length(pos1 - pos2);
}