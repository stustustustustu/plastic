#include "Projectile.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

int Projectile::ACTIVE_PROJECTILES = 0;

Projectile::Projectile(std::vector<float> position, ProjectileType type, std::vector<float> direction) : Entity(position), type(type), direction(direction), target(nullptr), active(true) {
    creationTime = std::chrono::steady_clock::now();
    ACTIVE_PROJECTILES++;

    switch (type) {
        case LASER:
            speed = 0.0f; // Instant, no movement
            break;
        case AMMO:
            speed = 10.0f; // Fast-moving projectile
            break;
        case HOMING_MISSILE:
            speed = 2.0f; // Slow-moving projectile
            splashRadius = 25.0f; // Splash damage radius
            break;
        case BOMB:
            speed = 5.0f; // Medium speed
            splashRadius = 35.0f; // Splash damage radius
            break;
    }
}

Projectile::Projectile(std::vector<float> position, ProjectileType type, std::shared_ptr<Enemy> target) : Entity(position), type(type), target(&target), active(true) {
    creationTime = std::chrono::steady_clock::now();
    ACTIVE_PROJECTILES++;

    direction = {
        target->getPosition()[0] - getPosition()[0],
        target->getPosition()[1] - getPosition()[1]
    };

    switch (type) {
        case LASER:
            speed = 0.0f; // Instant, no movement
            break;
        case AMMO:
            speed = 10.0f; // Fast-moving projectile
            break;
        case HOMING_MISSILE:
            speed = 2.0f; // Slow-moving projectile
            splashRadius = 25.0f; // Splash damage radius
            break;
        case BOMB:
            speed = 5.0f; // Medium speed
            splashRadius = 35.0f; // Splash damage radius
            break;
    }
}

void Projectile::update() {
    if (!active) return;

    move();
    //checkHit();

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - creationTime;

    if (elapsedTime.count() > 5.0f) { // Projectile expires after 5 seconds
        active = false;
        ACTIVE_PROJECTILES--;
    }
}

void Projectile::render() {
    if (!active) return;

    int hex;
    switch (type) {
        case LASER:
            hex = 0xFF0000; // Red for laser
            break;
        case AMMO:
            hex = 0x00FF00; // Green for ammo
            break;
        case HOMING_MISSILE:
            hex = 0x0000FF; // Blue for homing missile
            break;
        case BOMB:
            hex = 0xFFFF00; // Yellow for bomb
            break;
    }

    game->renderer->DrawSpriteSheet(*game->texture, glm::vec2(getRenderPosition().at(0), getRenderPosition().at(1)), 0, 32, 32, glm::vec2(16.0f), 0, HEXtoRGB(hex));
}

bool Projectile::isExpired() const {
    return !active;
}

void Projectile::move() {
    if (!target) return;

    if (direction.size() != 2) {
        throw std::runtime_error("Invalid direction vector.");
    }

    switch (type) {
        case LASER:
            // Laser is instant, no movement needed
            break;
        case AMMO:
        case BOMB:
            setPosition({getPosition().at(0) + direction[0] * speed, getPosition().at(1) + direction[1] * speed});
        break;
        case HOMING_MISSILE:
            if (target) {
                std::vector<float> direction = {
                    (*target)->getPosition()[0] - getPosition()[0],
                    (*target)->getPosition()[1] - getPosition()[1]
                };
                float magnitude = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
                if (magnitude > 0) {
                    direction[0] /= magnitude;
                    direction[1] /= magnitude;
                }

                direction[0] += (rand() % 100 - 50) / 100.0f;
                direction[1] += (rand() % 100 - 50) / 100.0f;

                setPosition({getPosition()[0] + direction[0] * speed, getPosition()[1] + direction[1] * speed});
            } else {
                active = false;
                ACTIVE_PROJECTILES--;
            }
        break;
    }
}

void Projectile::checkHit() {
    if (type == HOMING_MISSILE && target) {
        if (Collision::satCollision(getBounds(), target -> get() -> getBounds())) {
            (*target) -> hit(getDamage(), false);
            if (splashRadius > 0) {
                createSplash(getDamage(), splashRadius);
            }
            active = false;
            ACTIVE_PROJECTILES--;
        }
    } else {
        // Check collision with all enemies for non-homing projectiles
        for (auto& enemy : *game->enemies) {
            if (Collision::satCollision(getBounds(), enemy.getBounds())) {
                enemy.hit(getDamage(), false);
                if (type == BOMB && splashRadius > 0) {
                    createSplash(getDamage(), splashRadius);
                }
                active = false;
                ACTIVE_PROJECTILES--;
                break;
            }
        }
    }
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