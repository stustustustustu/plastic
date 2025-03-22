#include "Turret.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

Turret::Turret(glm::vec2 position, TurretType type)
    : Entity(position), type(type), target(NULL), currentAngle(0.0f), timeSinceLastShot(0.0f), lastUpdateTime(std::chrono::steady_clock::now()) {
    switch (type) {
        case TurretType::LASER:
            fireRate = 10.0f;
            setDamage(1.0f);
            cost = 20;
            break;
        case TurretType::RIFLE:
            fireRate = 20.0f;
            setDamage(5.0f);
            cost = 50;
            break;
        case TurretType::BOMB:
            fireRate = 2.0f;
            setDamage(25.0f);
            cost = 100;
            break;
    }

    rotationSpeed = 90.0f;
}

void Turret::findTarget() {
    float minDistance = std::numeric_limits<float>::max();
    target = NULL;

    for (const auto &enemy : *game -> getCurrentWorld() -> enemies) {
        float distance = calculateDistance(getPosition(), enemy.getPosition());
        if (distance < minDistance) {
            minDistance = distance;
            target = const_cast<Enemy*>(&enemy);
        }
    }
}

void Turret::rotateTowardsTarget() {
    if (!target) return;

    auto pos = getPosition();
    auto targetPos = target -> getPosition();
    float targetAngle = std::atan2(targetPos.y - pos.y, targetPos.x - pos.x) * 180.0f / M_PI;

    float angleDiff = targetAngle - currentAngle;
    if (angleDiff > 180.0f) angleDiff -= 360.0f;
    if (angleDiff < -180.0f) angleDiff += 360.0f;

    float angleStep = rotationSpeed;

    if (std::abs(angleDiff) < angleStep)
        currentAngle = targetAngle;
    else
        currentAngle += (angleDiff > 0 ? angleStep : -angleStep);

    if (currentAngle > 360.0f) currentAngle -= 360.0f;
    if (currentAngle < 0.0f) currentAngle += 360.0f;
}

void Turret::shoot() {
    if (!target) return;

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;

    if (elapsedTime.count() < (1.0f / fireRate)) {
        return;
    }

    lastUpdateTime = currentTime;

    timeSinceLastShot = 0.0f;

    if (type == TurretType::LASER) {
        target -> hit(getDamage(), false);
    } else {
        std::shared_ptr<Enemy> sharedTarget = std::make_shared<Enemy>(*target);
        std::unique_ptr<Projectile> newProjectile = nullptr;

        auto pos = getPosition() + glm::vec2(16, 16);

        switch (type) {
            case TurretType::RIFLE:
                newProjectile = std::make_unique<Projectile>(pos, AMMO, sharedTarget, getDamage());
                break;
            case TurretType::BOMB:
                newProjectile = std::make_unique<Projectile>(pos, BOMB, sharedTarget, getDamage());
                break;
            default:
                break;
        }

        if (newProjectile) {
            game -> getCurrentWorld() -> projectiles.push_back(std::move(newProjectile));
        }
    }
}

void Turret::render(Texture *texture) const {
    int hex;
    switch (type) {
        case TurretType::LASER:
            if (target)
                game -> renderer -> DrawLine(getRenderPosition() + glm::ivec2(16), target -> getRenderPosition() + glm::ivec2(16), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            hex = 0xFF5733;
            break;
        case TurretType::RIFLE:
            hex = 0xF2EE0A;
            break;
        case TurretType::BOMB:
            hex = 0x323232;
            break;
    }

    game -> renderer -> DrawSpriteSheet(*texture, getRenderPosition(), 2, 32, 32, glm::vec2(32.0f), currentAngle, HEXtoRGB(hex));
}

int Turret::getCost(TurretType type) {
    switch (type) {
        case TurretType::LASER:
            return 20;
        case TurretType::RIFLE:
            return 50;
        case TurretType::BOMB:
            return 100;
        default:
            return 0;
    }
}

TurretType Turret::getType() const {
    return this -> type;
}

float Turret::calculateDistance(const glm::vec2& a, const glm::vec2& b) {
    return glm::length(b - a);
}