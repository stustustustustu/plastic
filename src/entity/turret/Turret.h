#ifndef TURRET_H
#define TURRET_H

#include "../Entity.h"
#include "../../utils/render/RenderUtils.h"

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
        Entity* target;

        float fireRate;
        float timeSinceLastShot;
        std::chrono::steady_clock::time_point lastUpdateTime;

    public:
        Turret(std::vector<float> position, TurretType type);

        void findTarget(const std::vector<Entity>& enemies);

        void rotateTowardsTarget();

        void shoot();

        void drawTargetLine() const;

        void render() const;

    private:
        static float calculateDistance(const std::vector<float>& a, const std::vector<float>& b);
};

#endif //TURRET_H
