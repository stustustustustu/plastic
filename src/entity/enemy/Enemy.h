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
        std::pair<glm::vec2, std::chrono::milliseconds> spawn;

    public:
        Enemy(EnemyType type, const glm::vec2& position, float health, float damage, float speed);
        void moveTowards(const glm::vec2& targetPos);

        static std::map<EnemyType, std::tuple<float, float, float, float>> getEnemyData();
        EnemyType getType() const;

        std::string getTypeString() const;
        std::string getInfoString() const;

        float getSizeScale() const {
            float healthRatio = std::get<1>(getEnemyData().at(type)) / getMaxHealth();
            return 0.5f + healthRatio * 0.75f;
        }

        void drawEntity() const;

        static std::vector<Enemy> generateEnemies(int index, int totalWeight);

        void setSpawn(const glm::vec2& position, std::chrono::milliseconds time) { spawn = {position, time}; }
        std::pair<glm::vec2, std::chrono::milliseconds> getSpawn() const { return spawn; }

    private:
        static float calculateDistance(const glm::vec2& pos1, const glm::vec2& pos2);

        static std::map<EnemyType, std::string> getEnemyTypeStrings();


};

#endif //ENEMY_H
