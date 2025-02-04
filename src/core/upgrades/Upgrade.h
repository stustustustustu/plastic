#ifndef UPGRADE_H
#define UPGRADE_H

#include "../../config.h"

enum UpgradeType {
    DAMAGE, // increases damage
    HEALTH, // heals for some health on upgrade, increases max health
    SPEED, // increases speed
    SPECIAL // lifesteal and so on
};

class Upgrade {
    private:
        int cost;
        UpgradeType type;
        float multiplier;
        std::string description;
    public:
        Upgrade(int cost, UpgradeType type, float increase, const std::string &description);
        int getCost() const;
        void setCost(int cost);
        float getMultiplier() const;
        void setMultiplier(float multiplier);

        UpgradeType getType() const;
        void setType(UpgradeType type);

        const std::string &getDescription() const;
        void setDescription(const std::string &description);
};



#endif //UPGRADE_H
