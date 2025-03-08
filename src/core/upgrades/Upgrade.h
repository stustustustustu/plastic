#ifndef UPGRADE_H
#define UPGRADE_H

#include "../../config.h"

enum UpgradeType {
    // player
    DAMAGE,
    HEALTH,
    SHIELD,
    SPEED,
    SPECIAL,

    // turret
    TURRET_DAMAGE,
    TURRET_RANGE,
    TURRET_FIRERATE,
    TURRET_SPECIAL
};

class Upgrade {
    protected:
        std::string name;
        int cost;
        UpgradeType type;
        float multiplier;
        std::string description;
        bool unlocked = false; // mostly for turrets
        bool mutuallyExclusive = false;

    public:
        Upgrade(const std::string &name, int cost, UpgradeType type, float increase, const std::string &description, bool mutuallyExclusive = false);

        const std::string &getName() const;
        void setName(const std::string &name);

        int getCost() const;
        void setCost(int cost);

        float getMultiplier() const;
        void setMultiplier(float multiplier);

        UpgradeType getType() const;
        void setType(UpgradeType type);

        const std::string &getDescription() const;
        void setDescription(const std::string &description);

        bool isUnlocked() const;
        void setUnlocked(bool unlocked);

        bool isTurretUpgrade() const;

        bool isMutuallyExclusive() const;
};



#endif //UPGRADE_H
