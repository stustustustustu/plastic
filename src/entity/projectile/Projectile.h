#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../enemy/Enemy.h"
#include <glm/glm.hpp>

enum ProjectileType {
    LASER,
    AMMO,
    HOMING_MISSILE,
    BOMB
};

class Projectile : public Entity {
    private:
        ProjectileType type;
        std::shared_ptr<Enemy> target;
        glm::vec2 direction;

        float splashRadius = 0; // for homing missiles and bombs

        bool active = true;
        bool marked = false;

        std::chrono::steady_clock::time_point creationTime;

    public:
        static constexpr int MAX_PROJECTILES = 100;
        static int ACTIVE_PROJECTILES;

    public:
        Projectile(glm::vec2 position, ProjectileType type, glm::vec2 direction, float damage);
        Projectile(glm::vec2 position, ProjectileType type, std::shared_ptr<Enemy> target, float damage);

        void update();
        void render();

        bool isMarked() const { return this -> marked; }

        void mark() { this -> marked = true; }

        private:
        void move(); // based on type
        void updateBounds();
};

#endif // PROJECTILE_H