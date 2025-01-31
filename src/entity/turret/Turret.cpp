#include "Turret.h"

Turret::Turret(std::vector<float> position, TurretType type) : Entity(position), type(type), target(NULL), currentAngle(0.0f), timeSinceLastShot(0.0f), lastUpdateTime(std::chrono::steady_clock::now()) {
    switch (type) {
        case TurretType::LASER:
            fireRate = 50.0f;
            setDamage(2.0f);
            break;
        case TurretType::RIFLE:
            fireRate = 25.0f;
            setDamage(5.0f);
            break;
        case TurretType::BOMB:
            fireRate = 15.0f;
            setDamage(10.0f);
            break;
    }

    rotationSpeed = 90.0f;
}

void Turret::findTarget(const std::vector<Entity>& enemies) {
    float minDistance = std::numeric_limits<float>::max();
    target = NULL;

    for (const auto& enemy : enemies) {
        float distance = calculateDistance(getPosition(), enemy.getPosition());
        if (distance < minDistance) {
            minDistance = distance;
            target = const_cast<Entity*>(&enemy);
        }
    }
}

void Turret::rotateTowardsTarget() {
    if (!target) return;

    auto pos = getPosition();
    auto targetPos = target->getPosition();
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

    target -> hit(this -> getDamage(), false);

}

void Turret::drawTargetLine(Renderer &renderer) const {
    if (!target) return;

    if (target -> getPosition().size() > 0) {
        Entity::drawTargetLine(renderer, *target, 2.0f, 0xFF0000);
    }
}

void Turret::render(Renderer &renderer) const {
    int hex;
    switch (type) {
        case TurretType::LASER:
            hex = 0xFF5733;
            break;
        case TurretType::RIFLE:
            hex = 0xF2EE0A;
            break;
        case TurretType::BOMB:
            hex = 0x323232;
            break;
    }
    renderer.DrawRectangle(getRenderPosition().at(0) - 16, getRenderPosition().at(1) - 16, 32, 32, hex);
}

float Turret::calculateDistance(const std::vector<float>& a, const std::vector<float>& b) {
    return std::sqrt(std::pow(b[0] - a[0], 2) + std::pow(b[1] - a[1], 2));
}