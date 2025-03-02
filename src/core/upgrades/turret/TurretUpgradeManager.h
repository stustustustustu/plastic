#ifndef TURRETUPGRADEMANAGER_H
#define TURRETUPGRADEMANAGER_H

#include "../Upgrade.h"
#include "../src/entity/turret/Turret.h"

class TurretUpgradeManager {
    private:
        std::vector<Upgrade*> upgrades;
        std::map<Upgrade*, std::vector<Upgrade*>> prerequisites;

    public:
        TurretUpgradeManager();
        ~TurretUpgradeManager();

        void addUpgrade(Upgrade* upgrade, const std::vector<Upgrade*>& prerequisites = {});
        const std::vector<Upgrade*>& getAvailableUpgrades() const;

        bool unlockUpgrade(Turret& turret, const std::string& upgradeName);

        void applyUpgradeEffect(Turret &turret, Upgrade *upgrade);

        std::map<Upgrade*, std::vector<Upgrade*>> getSkillTree() const;
};



#endif //TURRETUPGRADEMANAGER_H
