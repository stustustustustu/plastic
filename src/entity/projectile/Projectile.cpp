#include "Projectile.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

int Projectile::ACTIVE_PROJECTILES = 0;

Projectile::Projectile(glm::vec2 position, ProjectileType type, glm::vec2 direction, float damage)
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

Projectile::Projectile(glm::vec2 position, ProjectileType type, std::shared_ptr<Enemy> target, float damage)
    : Entity(position), type(type), target(target) {

    if (type != LASER) {
        creationTime = std::chrono::steady_clock::now();
        ACTIVE_PROJECTILES++;
    }

    direction = {
        (target -> getPosition().x + 16) - (getPosition().x + 16),
        (target -> getPosition().y + 16) - (getPosition().y + 16)
    };

    switch (type) {
        case LASER:
            break;
        case AMMO:
            this -> setSpeed(10.0f);
            break;
        case HOMING_MISSILE:
            this -> setSpeed(2.0f);
            splashRadius = 50.0f;
            break;
        case BOMB:
            this -> setSpeed(3.0f);
            splashRadius = 75.0f;
            break;
    }

    this -> setDamage(damage);
    updateBounds();
}

void Projectile::update() {
    if (!active || marked) return;

    move();

    for (auto &enemy : *game -> getCurrentWorld() -> enemies) {
        if (type != LASER) {
            if (Collision::AABBCollision(getBounds(), enemy.getBounds())) {
                enemy.hit(getDamage(), false);

                if ((type == BOMB || type == HOMING_MISSILE) && splashRadius > 0) {
                    std::unique_ptr<Explosion> explosion = std::make_unique<Explosion>(getPosition(), splashRadius, getDamage(), 2);
                    game -> getCurrentWorld() -> explosions.push_back(std::move(explosion));
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
        if (type == BOMB || type == HOMING_MISSILE) {
            std::unique_ptr<Explosion> explosion = std::make_unique<Explosion>(getPosition(), splashRadius, getDamage(), 2);
            game -> getCurrentWorld() -> explosions.push_back(std::move(explosion));
        }

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
        game -> renderer -> DrawSpriteSheet(*game -> texture, getRenderPosition(), 0, 32, 32, size, 0, HEXtoRGB(hex));
    }
}

void Projectile::move() {
    if (type == LASER) return;

    if (type == HOMING_MISSILE && target) {
        direction = {
            (target -> getPosition().x + 16) - (getPosition().x + 16),
            (target -> getPosition().y + 16) - (getPosition().y + 16)
        };
    }

    // Normalize direction
    float magnitude = glm::length(direction);
    if (magnitude > 0) {
        direction /= magnitude;
    }

    // Move the projectile
    setPosition(getPosition() + direction * getSpeed());

    updateBounds();
}

void Projectile::updateBounds() {
    switch (type) {
        case LASER:
            setBounds({}); // laser has no bounds
        break;
        case AMMO:
            setBounds({
            {getPosition().x - 4, getPosition().y - 4},
            {getPosition().x + 4, getPosition().y + 4}
            });
        break;
        case HOMING_MISSILE:
            setBounds({
            {getPosition().x - 8, getPosition().y - 8},
            {getPosition().x + 8, getPosition().y + 8}
            });
        break;
        case BOMB:
            setBounds({
            {getPosition().x - 12, getPosition().y - 12},
            {getPosition().x + 12, getPosition().y + 12}
            });
        break;
    }
}