#ifndef UPGRADEPATH_H
#define UPGRADEPATH_H

#include "../Upgrade.h"
#include "../../../config.h"
#include "../../../utils/math/number/number.h"

class UpgradePath {
    private:
        UpgradeType type;
        std::vector<Upgrade> upgrades;
        int level = 0;

    private:
        std::string generateName() const;
        int generateCost() const;
        float generateMultiplier() const;
        std::string generateDescription(float multiplier) const;

    public:
        UpgradePath(UpgradeType type);

        Upgrade generateNextUpgrade();

        UpgradeType getType() const;

        const std::vector<Upgrade> &getUpgrades() const;
        const Upgrade &getCurrentUpgrade() const;
        const Upgrade &getNextUpgrade();
        void upgrade();

        int getLevel() const;
        void setLevel(int level);
};

#endif //UPGRADEPATH_H
