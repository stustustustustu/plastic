#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../enemy/Enemy.h"

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
        std::vector<float> direction;

        float splashRadius = 0; // for homing missiles and bombs

        bool active = true;
        bool marked = false;

        std::chrono::steady_clock::time_point creationTime;

    public:
        static constexpr int MAX_PROJECTILES = 100;
        static int ACTIVE_PROJECTILES;

    public:
        Projectile(std::vector<float> position, ProjectileType type, std::vector<float> direction, float damage);
        Projectile(std::vector<float> position, ProjectileType type, std::shared_ptr<Enemy> target, float damage);

        void update();
        void render();

        bool isMarked() const { return this -> marked; }

        void mark() { this -> marked = true; }

    private:
        void move(); // based on type
        void createSplash(float damage, float radius); // for homing missiles and bombs
        void updateBounds();
};

#endif //PROJECTILE_H
