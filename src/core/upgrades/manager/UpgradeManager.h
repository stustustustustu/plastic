#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include "UpgradePath.h"
#include "../../inventory/Inventory.h"
#include "../../../config.h"

class UpgradeManager {
    private:
        std::map<UpgradeType, UpgradePath> paths;
        Inventory& inventory;
    public:
        explicit UpgradeManager(Inventory& inventory);

        std::map<UpgradeType, UpgradePath> getPaths() const;

        void addPath(UpgradeType type);
        bool upgrade(UpgradeType type);

        float getMultiplier(UpgradeType type) const;
        int getCurrentLevel(UpgradeType type) const;
        int getCost(UpgradeType type, int level) const;

        bool hasUpgradePath(UpgradeType type) const;

        void loadPaths(const std::map<UpgradeType, int> &levels);
};

#endif //UPGRADEMANAGER_H
