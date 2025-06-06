#include "Entity.h"

#include <windows.h>

#include "../Game.h"

const auto game = Game::getInstance();

/**
 * @brief Constructor to initialize the Entity's attributes.
 *
 * @param position A pair of floats representing the Entity's center position.
 * @param speed The speed of the Entity (default is 1.0).
 * @param damage The damage dealt by the Entity (default is 1.0).
 * @param health The current health of the Entity (default is 100.0).
 * @param shield The current shield of the Entity (default is 0.0).
 * @param level The level of the Entity (default is 1).
 * @param coins The current coins of the Entity (default is 100).
 * @param targets The number of current targets (default is 0).
 */
Entity::Entity(
    glm::vec2 position,
    float speed,
    float damage,

    float health,
    float maxHealth,

    float shield,
    float maxShield,

    int level,
    int coins,

    int targets,
    int maxTargets,

    const std::vector<glm::vec2> bounds
) : position(position), speed(speed), damage(damage), health(health), maxHealth(maxHealth), shield(shield), maxShield(maxShield), level(level), coins(coins), targets(targets), maxTargets(maxTargets), bounds(bounds) {}

void Entity::drawEntity(Texture *texture) const {
    game -> renderer -> DrawSpriteSheet(*texture, getPosition(), 0, 32, 32);
}

/**
 * @brief Draws a line from the entity to another on screen.
 *
 * @param hex The color of the line to draw.
 */
void Entity::drawTargetLine(Entity target, float thickness, int hex) const {
    game -> renderer -> DrawLine(getPosition() + glm::vec2(16), target.getPosition() + glm::vec2(16), thickness, HEXtoRGB(hex));
}

std::vector<glm::vec2> Entity::getBounds() const {
    return this -> bounds;
}

void Entity::setBounds(const std::vector<glm::vec2> &bounds) {
    this -> bounds = bounds;
}

bool Entity::checkCollision(const Entity &other) const {
    return Collision::satCollision(bounds, other.getBounds());
}

void Entity::updateBounds() {
    if (bounds.empty()) return; // No bounds to update

    float halfWidth = (bounds[1][0] - bounds[0][0]) / 2.0f;
    float halfHeight = (bounds[1][1] - bounds[0][1]) / 2.0f;

    bounds = {
        {getPosition()[0] - halfWidth, getPosition()[1] - halfHeight}, // Min bounds
        {getPosition()[0] + halfWidth, getPosition()[1] + halfHeight}  // Max bounds
    };
}

/**
 * @brief Getter for the Entity's position.
 *
 * @return A vector of floats representing the Entity's position.
 */
glm::vec2 Entity::getPosition() const {
    return this -> position;
}

/**
 * @brief Getter for the Entity's render position.
 *
 * @return A vector of integers representing the Entity's render position.
 */
glm::ivec2 Entity::getRenderPosition() const {
    return {static_cast<int>(std::round(position.x)), static_cast<int>(std::round(position.y))};
}

/**
 * @brief Setter for the Entity's position.
 *
 * @param position A vector representing the new position of the Entity.
 */
void Entity::setPosition(glm::vec2 position) {
    this -> position = position;
}

/**
 * @brief Moves the Entity by a given delta in both x and y directions.
 *
 * @param delta A vector representing the change in position.
 */
void Entity::move(const glm::ivec2& delta) {
    position.x += delta.x * getSpeed();
    position.y += delta.y * getSpeed();
}

void Entity::move(const glm::vec2& delta) {
    position.x += delta.x * getSpeed();
    position.y += delta.y * getSpeed();
}

/**
 * @brief Getter for the Entity's speed.
 *
 * @return The speed value of the Entity.
 */
float Entity::getSpeed() const {
    return this -> speed;
}

/**
 * @brief Setter for the Entity's speed.
 *
 * @param speed The new speed value to be set for the Entity.
 */
void Entity::setSpeed(float speed) {
    this -> speed = speed;
}

/**
 * @brief Getter for the Entity's damage value.
 *
 * @return The damage value of the Entity.
 */
float Entity::getDamage() const {
    return this -> damage;
}

/**
 * @brief Setter for the Entity's damage value.
 *
 * @param damage The new damage value to be set for the Entity.
 */
void Entity::setDamage(float damage) {
    this -> damage = damage;
}

/**
 * @brief Getter for the Entity's current health.
 *
 * @return The current health value of the Entity.
 */
float Entity::getHealth() const {
    return this -> health;
}

/**
 * @brief Setter for the Entity's health value.
 *
 * @param health The new health value to be set for the Entity. Health cannot be negative.
 */
void Entity::setHealth(float health) {
    this -> health = (health >= 0) ? health : 0;
}

/**
 * @brief Getter for the Entity's maximum health.
 *
 * @return The maximum health value of the Entity.
 */
float Entity::getMaxHealth() const {
    return this -> maxHealth;
}

/**
 * @brief Setter for the Entity's maximum health value.
 *
 * @param health The new maximum health value to be set. Max health cannot be negative.
 */
void Entity::setMaxHealth(float health) {
    if (health >= 0) {
        this -> maxHealth = health;
    }
}

/**
 * @brief Getter for the Entity's shield value.
 *
 * @return The current shield value of the Entity.
 */
float Entity::getShield() const {
    return this -> shield;
}

/**
 * @brief Setter for the Entity's shield value.
 *
 * @param shield The new shield value to be set for the Entity. Shield cannot be negative.
 */
void Entity::setShield(float shield) {
    this -> shield = (shield >= 0) ? shield : 0;
}

/**
 * @brief Getter for the Entity's maximum shield value.
 *
 * @return The maximum shield value of the Entity.
 */
float Entity::getMaxShield() const {
    return this -> maxShield;
}

/**
 * @brief Setter for the Entity's maximum shield value.
 *
 * @param shield The new maximum shield value to be set. Max shield cannot be negative.
 */
void Entity::setMaxShield(float shield) {
    if (shield >= 0) {
        this -> maxShield = shield;
    }
}

/**
 * @brief Handles damage taken by the Entity, affecting shield and health.
 *
 * If the Entity has a shield and the damage is not true damage, the shield is damaged first.
 * If the shield is depleted, the remaining damage affects health. If the damage is true damage,
 * it bypasses the shield and directly affects health.
 *
 * @param damage The amount of damage to be taken.
 * @param trueDamage If true, the damage bypasses the shield and directly reduces health.
 */
void Entity::hit(float damage, bool trueDamage) {
    if (damage <= 0) {
        std::cerr << "Damage taken cannot be negative or 0." << std::endl;
        return;
    }

    // Damage the shield first (if trueDamage == false)
    if (getShield() > 0 && !trueDamage) {
        if (getShield() >= damage) {
            setShield(getShield() - damage);
        } else {
            damage -= getShield();
            setShield(0);
            setHealth(getHealth() - damage);
        }
    } else {
        setHealth(getHealth() - damage);
    }

    // No negative health
    if (getHealth() < 0) {
        setHealth(0);
    }
}

/**
 * @brief Heals the Entity by a certain amount.
 * If shield is true, the shield will be healed; otherwise, health is healed.
 *
 * @param healing The amount of health or shield to be restored.
 * @param shield If true, restores shield instead of health.
 */
void Entity::heal(float healing, bool shield) {
    if (healing <= 0) {
        std::cerr << "Healing cannot be negative or 0." << std::endl;
        return;
    }

    if (!shield) {
        if (getHealth() + healing > getMaxHealth()) {
            setHealth(getMaxHealth());
        } else {
            setHealth(getHealth() + healing);
        }
    } else {
        if (getShield() + healing > getMaxShield()) {
            setShield(getMaxShield());
        } else {
            setShield(getShield() + healing);
        }
    }
}

/**
 * @brief Getter for the Entity's current level.
 *
 * @return The level of the Entity.
 */
int Entity::getLevel() const {
    return this -> level;
}

/**
 * @brief Setter for the Entity's level.
 *
 * @param level The new level to be set for the Entity. Level must be greater than 0.
 */
void Entity::setLevel(int level) {
    if (level > 0) {
        this -> level = level;
    }
}

int Entity::getCoins() const {
    return this -> coins;
}

void Entity::setCoins(int coins) {
    this -> coins = coins;
}

void Entity::addCoins(int coins) {
    this -> coins += coins;
}

bool Entity::spendCoins(int coins) {
    if (this -> coins - coins >= 0) {
        this -> coins -= coins;
        return true;
    }
    std::cerr << "Insufficient funds." << std::endl;
    return false;
}

bool Entity::takeCoins(Entity &target, float percentage) {
    if (percentage < 0.0f || percentage > 1.0f) {
        std::cerr << "Invalid percentage. Must be between 0 and 1." << std::endl;
        return false;
    }

    int amount = static_cast<int>(target.getCoins() * percentage);
    if (amount <= 0) {
        return false;
    }

    if (target.spendCoins(amount)) {
        this -> addCoins(amount);
        return true;
    }

    return false;
}

void Entity::applyUpgrade(Upgrade upgrade) {
    switch (upgrade.getType()) {
        case HEALTH:
            setMaxHealth(getMaxHealth() * upgrade.getMultiplier());
            setHealth(getHealth() * upgrade.getMultiplier());
            break;
        case SHIELD:
            setMaxShield(getMaxShield() * upgrade.getMultiplier());
            setShield(getShield() * upgrade.getMultiplier());
            break;
        case DAMAGE:
            setDamage(getDamage() * upgrade.getMultiplier());
            break;
        case SPEED:
            setSpeed(getSpeed() * upgrade.getMultiplier());
            break;
        default:
            break;
    }
}