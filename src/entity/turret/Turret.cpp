#include "Turret.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

Turret::Turret(std::vector<float> position, TurretType type) : Entity(position), type(type), target(NULL), currentAngle(0.0f), timeSinceLastShot(0.0f), lastUpdateTime(std::chrono::steady_clock::now()) {
    switch (type) {
        case TurretType::LASER:
            fireRate = 50.0f; // instant
            setDamage(2.0f);
            break;
        case TurretType::RIFLE:
            fireRate = 20.0f;
            setDamage(5.0f);
            break;
        case TurretType::BOMB:
            fireRate = 5.0f;
            setDamage(15.0f);
            break;
    }

    rotationSpeed = 90.0f;
}

void Turret::findTarget() {
    float minDistance = std::numeric_limits<float>::max();
    target = NULL;

    for (const auto &enemy : *game -> enemies) {
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
    float targetAngle = std::atan2(targetPos[1] - pos[1], targetPos[0] - pos[0]) * 180.0f / M_PI;

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

    std::shared_ptr<Enemy> sharedTarget = std::make_shared<Enemy>(*target);
    std::unique_ptr<Projectile> newProjectile = nullptr;
    switch (type) {
        case TurretType::LASER:
            newProjectile = std::make_unique<Projectile>(getPosition(), LASER, sharedTarget, getDamage());
        break;
        case TurretType::RIFLE:
            newProjectile = std::make_unique<Projectile>(getPosition(), AMMO, sharedTarget, getDamage());
        break;
        case TurretType::BOMB:
            newProjectile = std::make_unique<Projectile>(getPosition(), BOMB, sharedTarget, getDamage());
        break;
    }

    if (newProjectile) {
        game -> projectiles.push_back(std::move(newProjectile));
    }
}

void Turret::render(Texture *texture) const {
    int hex;
    switch (type) {
        case TurretType::LASER:
            if (target && target -> getRenderPosition().size() > 0 && target -> getRenderPosition().capacity() > 0)
                game -> renderer -> DrawLine(glm::vec2(getRenderPosition().at(0) + 16, getRenderPosition().at(1) + 16), glm::vec2(target -> getRenderPosition().at(0) + 16, target -> getRenderPosition().at(1) + 16), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            hex = 0xFF5733;
            break;
        case TurretType::RIFLE:
            hex = 0xF2EE0A;
            break;
        case TurretType::BOMB:
            hex = 0x323232;
            break;
    }

    game -> renderer -> DrawSpriteSheet(*texture, glm::vec2(getRenderPosition().at(0), getRenderPosition().at(1)), 2, 32, 32, glm::vec2(32.0f), currentAngle, HEXtoRGB(hex));
}

float Turret::calculateDistance(const std::vector<float>& a, const std::vector<float>& b) {
    return std::sqrt(std::pow(b[0] - a[0], 2) + std::pow(b[1] - a[1], 2));
}