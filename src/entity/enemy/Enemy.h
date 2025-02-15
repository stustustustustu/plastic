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
        Enemy(EnemyType type, const std::vector<float>& position, float health, float damage, float speed);
        void moveTowards(const std::vector<float>& targetPos);

        static std::map<EnemyType, std::tuple<float, float, float, float>> getEnemyData();

        static std::vector<Enemy> generateEnemies(int index, int totalWeight);

    private:
        static float calculateDistance(const std::vector<float>& pos1, const std::vector<float>& pos2);

};

#endif //ENEMY_H
