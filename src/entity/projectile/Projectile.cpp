#include "Projectile.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

int Projectile::ACTIVE_PROJECTILES = 0;

Projectile::Projectile(std::vector<float> position, ProjectileType type, std::vector<float> direction, float damage)
    : Entity(position), type(type), direction(direction), active(true) {
    if (type != LASER) {
        creationTime = std::chrono::steady_clock::now();
        ACTIVE_PROJECTILES++;
    }

    switch (type) {
        case LASER:
            this -> setSpeed(0.0f); // Instant, no movement
            break;
        case AMMO:
            this -> setSpeed(10.0f); // Fast-moving projectile
            break;
        case HOMING_MISSILE:
            this -> setSpeed(2.0f); // Slow-moving projectile
            splashRadius = 25.0f; // Splash damage radius
            break;
        case BOMB:
            this -> setSpeed(5.0f); // Medium speed
            splashRadius = 35.0f; // Splash damage radius
            break;
    }
    this -> setDamage(damage);
    updateBounds();
}

Projectile::Projectile(std::vector<float> position, ProjectileType type, std::shared_ptr<Enemy> target, float damage)
    : Entity(position), type(type), target(target), active(true) {
    if (type != LASER) {
        creationTime = std::chrono::steady_clock::now();
        ACTIVE_PROJECTILES++;
    }

    direction = {
        (target->getPosition()[0] + 16) - (getPosition()[0] + 16),
        (target->getPosition()[1] + 16) - (getPosition()[1] + 16)
    };

    switch (type) {
        case LASER:
            this -> setSpeed(0.0f); // Instant, no movement
            break;
        case AMMO:
            this -> setSpeed(10.0f); // Fast-moving projectile
            break;
        case HOMING_MISSILE:
            this -> setSpeed(2.0f); // Slow-moving projectile
            splashRadius = 25.0f; // Splash damage radius
            break;
        case BOMB:
            this -> setSpeed(5.0f); // Medium speed
            splashRadius = 35.0f; // Splash damage radius
            break;
    }

    this -> setDamage(damage);
    updateBounds();
}

void Projectile::update() {
    if (!active || marked) return;

    // Move the projectile
    move();

    // Check for collisions with enemies
    if (type != LASER) { // Lasers don't need collision checks (they deal damage over time)
        for (auto& enemy : *game->enemies) {
            if (checkAABBCollision(getBounds(), enemy.getBounds())) {
                // Hit the enemy
                enemy.hit(getDamage(), false);

                // Handle splash damage for bombs and homing missiles
                if ((type == BOMB || type == HOMING_MISSILE) && splashRadius > 0) {
                    createSplash(getDamage(), splashRadius);
                }

                // Mark the projectile for removal
                mark();
                ACTIVE_PROJECTILES--;

                return; // Exit after the first collision
            }
        }
    }

    // Check if the projectile has expired
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - creationTime;

    if (elapsedTime.count() > 5.0f) { // Projectile expires after 5 seconds
        mark();
        ACTIVE_PROJECTILES--;
    }
}

void Projectile::render() {
    if (!active) return;

    int hex;
    switch (type) {
        case LASER:
            if (target) {
                game -> renderer -> DrawLine(
                    glm::vec2(getRenderPosition().at(0) + 16, getRenderPosition().at(1) + 16),
                    glm::vec2(target -> getRenderPosition().at(0) + 16, target -> getRenderPosition().at(1) + 16),
                    2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
                );
            }
            break;
        case AMMO:
            hex = 0x00FF00;
            break;
        case HOMING_MISSILE:
            hex = 0x0000FF;
            break;
        case BOMB:
            hex = 0xFFFF00;
            break;
    }

    if (type != LASER) {
        game -> renderer -> DrawSpriteSheet(*game->texture, glm::vec2(getRenderPosition().at(0), getRenderPosition().at(1)), 0, 32, 32, glm::vec2(16.0f), 0, HEXtoRGB(hex));
    }
}

bool Projectile::isExpired() const {
    return !active;
}

void Projectile::move() {
    if (type == LASER) return;

    if (type == HOMING_MISSILE && target) {
        direction = {
            (target->getPosition()[0] + 16) - (getPosition()[0] + 16),
            (target->getPosition()[1] + 16) - (getPosition()[1] + 16)
        };
    }

    // Normalize direction
    float magnitude = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
    if (magnitude > 0) {
        direction[0] /= magnitude;
        direction[1] /= magnitude;
    }

    // Move the projectile
    setPosition({
        getPosition()[0] + direction[0] * getSpeed(),
        getPosition()[1] + direction[1] * getSpeed()
    });

    updateBounds();
}

void Projectile::createSplash(float damage, float radius) {
    for (auto& enemy : *game->enemies) {
        float distance = std::sqrt(std::pow(enemy.getPosition()[0] - getPosition()[0], 2) +
                         std::pow(enemy.getPosition()[1] - getPosition()[1], 2));
        if (distance <= radius) {
            enemy.hit(damage * (1.0f - distance / radius), false); // Damage decreases with distance
        }
    }
}

bool Projectile::checkAABBCollision(const std::vector<std::vector<float>>& boundsA, const std::vector<std::vector<float>>& boundsB) {
    if (boundsA.empty() || boundsB.empty()) return false; // Early return if bounds are invalid

    // Get the min and max coordinates for each bounding box
    float minA_x = boundsA[0][0];
    float minA_y = boundsA[0][1];
    float maxA_x = boundsA[1][0];
    float maxA_y = boundsA[1][1];

    float minB_x = boundsB[0][0];
    float minB_y = boundsB[0][1];
    float maxB_x = boundsB[1][0];
    float maxB_y = boundsB[1][1];

    // Check for overlap
    return (minA_x <= maxB_x && maxA_x >= minB_x &&
            minA_y <= maxB_y && maxA_y >= minB_y);
}

void Projectile::updateBounds() {
    switch (type) {
        case LASER:
            setBounds({}); // laser has no bounds
        break;
        case AMMO:
            setBounds({
            {getPosition()[0] - 4, getPosition()[1] - 4},
            {getPosition()[0] + 4, getPosition()[1] + 4}
            });
        break;
        case HOMING_MISSILE:
            setBounds({
            {getPosition()[0] - 8, getPosition()[1] - 8},
            {getPosition()[0] + 8, getPosition()[1] + 8}
            });
        break;
        case BOMB:
            setBounds({
            {getPosition()[0] - 12, getPosition()[1] - 12},
            {getPosition()[0] + 12, getPosition()[1] + 12}
            });
        break;
    }
}
