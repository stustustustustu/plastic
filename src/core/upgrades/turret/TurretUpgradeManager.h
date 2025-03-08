#ifndef TURRETUPGRADEMANAGER_H
#define TURRETUPGRADEMANAGER_H

#include "../Upgrade.h"
#include "../src/entity/turret/Turret.h"

class TurretUpgradeManager {
    private:
        std::map<TurretType, std::vector<Upgrade*>> upgrades;

        std::map<Upgrade*, std::vector<Upgrade*>> prerequisites;
        std::map<Upgrade*, std::vector<Upgrade*>> mutuallyExclusiveGroups;

    public:
        TurretUpgradeManager();
        ~TurretUpgradeManager();

        void addUpgrade(TurretType type, Upgrade* upgrade, const std::vector<Upgrade*>& prerequisites = {}, const std::vector<Upgrade*>& mutuallyExclusive = {});
        const std::vector<Upgrade*>& getAvailableUpgrades(TurretType) const;

        bool unlockUpgrade(Turret& turret, const std::string& upgradeName);

        void applyUpgradeEffect(Turret &turret, Upgrade *upgrade);

        std::map<Upgrade*, std::vector<Upgrade*>> getSkillTree(TurretType type) const;
};



#endif //TURRETUPGRADEMANAGER_H
