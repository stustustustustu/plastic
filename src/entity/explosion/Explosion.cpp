#include "Explosion.h"

#include "../../Game.h"

const auto game = Game::getInstance();

Explosion::Explosion(glm::vec2 position, float size, float damage, float duration) : Entity(position), size(size), damage(damage), duration(duration), elapsedTime(0.0) {
    applySplashDamage();
}

void Explosion::update() {
    elapsedTime += 0.2f;
    if (elapsedTime >= duration) {
        elapsedTime = duration;
    }
}

void Explosion::render() const {
    game -> renderer -> DrawRect(
        glm::vec2(getRenderPosition().x - size / 2, getRenderPosition().y - size / 2),
        glm::vec2(size),
        0, HEXtoRGB(0x000000)
    );
}

bool Explosion::isFinished() const {
    return elapsedTime >= duration;
}

void Explosion::applySplashDamage() const {
    for (auto &enemy : *game -> getCurrentWorld() -> enemies) {
        float distance = glm::length(enemy.getPosition() - getPosition());
        if (distance <= size) {
            float damageDealt = damage * (1.0f - distance / size);
            enemy.hit(damageDealt, false);
        }
    }
}