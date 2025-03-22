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

    public:
        Enemy(EnemyType type, const glm::vec2& position, float health, float damage, float speed);
        void moveTowards(const glm::vec2& targetPos);

        static std::map<EnemyType, std::tuple<float, float, float, float>> getEnemyData();
        EnemyType getType() const;

        static std::vector<Enemy> generateEnemies(int index, int totalWeight);

    private:
        static float calculateDistance(const glm::vec2& pos1, const glm::vec2& pos2);

};

#endif //ENEMY_H
