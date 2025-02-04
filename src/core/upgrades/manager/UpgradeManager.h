#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include "UpgradePath.h"
#include "../../inventory/Inventory.h"
#include "../../../config.h"

class UpgradeManager {
    public:
        explicit UpgradeManager(Inventory& inventory);

        void Init();

        void addPath(const std::string& name, const UpgradePath& path);
        bool upgrade(const std::string& name);

        float getMultiplier(const std::string& name) const;
        int getCurrentLevel(const std::string& name) const;
        int getCost(const std::string& name, int level) const;

        bool hasUpgradePath(const std::string& name) const;

    private:
        std::map<std::string, UpgradePath> paths;
        Inventory& inventory;
};


#endif //UPGRADEMANAGER_H
