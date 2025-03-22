#ifndef TURRET_H
#define TURRET_H

#include "../Entity.h"
#include "../enemy/Enemy.h"

enum class TurretType {
    LASER,
    BOMB,
    RIFLE
};

class Turret : public Entity {
    private:
        TurretType type;
        float rotationSpeed;
        float currentAngle;
        Enemy* target;
        int cost;

        float fireRate;
        float timeSinceLastShot;
        std::chrono::steady_clock::time_point lastUpdateTime;

    public:
        Turret(glm::vec2 position, TurretType type);

        void findTarget();

        void rotateTowardsTarget();

        void shoot();

        void render(Texture *texture) const;

        static int getCost(TurretType type);
        TurretType getType() const;

    private:
        static float calculateDistance(const glm::vec2& a, const glm::vec2& b);
};

#endif //TURRET_H
