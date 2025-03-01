#include "Projectile.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

int Projectile::ACTIVE_PROJECTILES = 0;

Projectile::Projectile(std::vector<float> position, ProjectileType type, std::vector<float> direction, float damage)
    : Entity(position), type(type), direction(direction) {

    if (type != LASER) {
        creationTime = std::chrono::steady_clock::now();
        ACTIVE_PROJECTILES++;
    }

    switch (type) {
        case LASER:
            break;
        case AMMO:
            this -> setSpeed(7.5f);
            break;
        case HOMING_MISSILE:
            this -> setSpeed(1.0f);
            splashRadius = 5.0f;
            break;
        case BOMB:
            this -> setSpeed(2.5f);
            splashRadius = 10.0f;
            break;
    }

    this -> setDamage(damage);
    updateBounds();
}

Projectile::Projectile(std::vector<float> position, ProjectileType type, std::shared_ptr<Enemy> target, float damage)
    : Entity(position), type(type), target(target) {

    if (type != LASER) {
        creationTime = std::chrono::steady_clock::now();
        ACTIVE_PROJECTILES++;
    }

    direction = {
        (target -> getPosition()[0] + 16) - (getPosition()[0] + 16),
        (target -> getPosition()[1] + 16) - (getPosition()[1] + 16)
    };

    switch (type) {
        case LASER:
            break;
        case AMMO:
            this -> setSpeed(10.0f);
            break;
        case HOMING_MISSILE:
            this -> setSpeed(2.0f);
            splashRadius = 25.0f;
            break;
        case BOMB:
            this -> setSpeed(3.0f);
            splashRadius = 35.0f;
            break;
    }

    this -> setDamage(damage);
    updateBounds();
}

void Projectile::update() {
    if (!active || marked) return;

    move();

    for (auto &enemy : *game -> enemies) {
        if (type != LASER) {
            if (Collision::AABBCollision(getBounds(), enemy.getBounds())) {
                enemy.hit(getDamage(), false);

                if ((type == BOMB || type == HOMING_MISSILE) && splashRadius > 0) {
                    createSplash(getDamage(), splashRadius);
                }

                mark();
                ACTIVE_PROJECTILES--;

                return;
            }
        } else if (target) {
            target -> hit(getDamage(), false);
        }
    }

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - creationTime;

    if (elapsedTime.count() > 1.5f || !target) {
        mark();
        ACTIVE_PROJECTILES--;
    }
}

void Projectile::render() {
    if (!active) return;

    int hex;
    glm::vec2 size;
    switch (type) {
        case LASER:
            break;
        case AMMO:
            hex = 0x00FF00;
            size = glm::vec2(8.0f);
            break;
        case HOMING_MISSILE:
            hex = 0x0000FF;
            size = glm::vec2(16.0f);
            break;
        case BOMB:
            hex = 0x000000;
            size = glm::vec2(16.0f);
            break;
    }

    if (type != LASER) {
        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(getRenderPosition().at(0), getRenderPosition().at(1)), 0, 32, 32, size, 0, HEXtoRGB(hex));
    }
}

void Projectile::move() {
    if (type == LASER) return;

    if (type == HOMING_MISSILE && target) {
        direction = {
            (target -> getPosition()[0] + 16) - (getPosition()[0] + 16),
            (target -> getPosition()[1] + 16) - (getPosition()[1] + 16)
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

void Projectile::createSplash(float damage, float radius) const {
    for (auto& enemy : *game -> enemies) {
        float distance = std::sqrt(std::pow(enemy.getPosition()[0] - getPosition()[0], 2) +
                         std::pow(enemy.getPosition()[1] - getPosition()[1], 2));
        if (distance <= radius) {
            enemy.hit(damage * (1.0f - distance / radius), false); // damage falloff over distance
        }
    }
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
