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
        std::shared_ptr<Enemy> *target;
        std::vector<float> direction;

        float speed;
        float splashRadius = 0; // for homing missiles and bombs

        bool active;

        std::chrono::steady_clock::time_point creationTime;

    public:
        static constexpr int MAX_PROJECTILES = 1000;
        static int ACTIVE_PROJECTILES;

    public:
        Projectile(std::vector<float> position, ProjectileType type, std::vector<float> direction);
        Projectile(std::vector<float> position, ProjectileType type, std::shared_ptr<Enemy> target);

        void update();
        void render();

        bool isExpired() const;

    private:
        void move(); // based on type
        void checkHit();
        void createSplash(float damage, float radius); // for homing missiles and bombs

};

#endif //PROJECTILE_H
