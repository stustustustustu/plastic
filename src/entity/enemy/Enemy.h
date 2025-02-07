#ifndef ENEMY_H
#define ENEMY_H

#include "../Entity.h"

enum class EnemyType {
    ABANDONED_SHIP,
    SHIPPING_CONTAINER,
    OIL_BARREL,
    BUOY_SYSTEM,
    PLASTIC_CANOE,
    INDUSTRIAL_PACKAGING,
    FISHERMANS_BARREL,
    FLOATING_PLATFORM,
    WATER_TANK,
    FISHING_GEAR,
    PLASTIC_BAG,
    TIRE,
    PLASTIC_BOTTLE
};

class Enemy : public Entity {
private:
    EnemyType type;
    float health;
    float speed;

    public:
        Enemy(const std::vector<float>& position, float health, float speed);

        void moveTowards(const std::vector<float>& targetPos);
        void moveTo(Entity& player);

        static void populate(std::vector<Enemy>& enemies, int count, GLFWwindow* window, float minDistance);

        static float calculateDistance(const std::vector<float>& pos1, const std::vector<float>& pos2);
        static bool isPositionValid(float x, float y, int halfWidth, int halfHeight, int border, int screenWidth, int screenHeight);
        static bool isPositionValidWithOthers(float x, float y, int halfWidth, int halfHeight, int border, int screenWidth, int screenHeight, const std::vector<Enemy>& enemies, float minDistance);
};

#endif //ENEMY_H
