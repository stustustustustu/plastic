#include "Explosion.h"

#include "../../Game.h"

const auto game = Game::getInstance();

Explosion::Explosion(std::vector<float> position, float size, float damage, float duration) : Entity(position), size(size), damage(damage), duration(duration), elapsedTime(0.0) {
    applySplashDamage();
}

void Explosion::update() {
    elapsedTime += 0.2f;
    if (elapsedTime >= duration) {
        elapsedTime = duration;
    }
}

void Explosion::render() const {
    game -> renderer -> DrawSpriteSheet(
        *game -> texture,
        glm::vec2(getRenderPosition().at(0) - size / 2, getRenderPosition().at(1) - size / 2),
        0, 32, 32,
        glm::vec2(size),
        0, HEXtoRGB(0x000000)
    );
}

bool Explosion::isFinished() const {
    return elapsedTime >= duration;
}

void Explosion::applySplashDamage() const {
    for (auto &enemy : *game -> enemies) {
        float distance = std::sqrt(std::pow(enemy.getPosition()[0] - getPosition()[0], 2) +
                         std::pow(enemy.getPosition()[1] - getPosition()[1], 2));

        if (distance <= size) {
            float damageDealt = damage * (1.0f - distance / size);
            enemy.hit(damageDealt, false);
        }
    }
}