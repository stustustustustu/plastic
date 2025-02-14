#ifndef TURRETMANAGER_H
#define TURRETMANAGER_H

#include "../Turret.h"
#include "../../../core/upgrades/manager/UpgradeManager.h"

class TurretManager {
    private:
        std::vector<std::shared_ptr<Turret>> turrets;

        bool showShop = false;
        bool showUpgrades = false;

        std::shared_ptr<Turret> selectedTurret;

    public:
        void update();
        void render();

        void placeTurret(TurretType type, const std::vector<float> &position);

        void openUpgradeMenu(std::shared_ptr<Turret> turret);
        void renderTurretShop();
        void renderUpgradeMenu();

        void handleClick(const glm::vec2 &mousePos);
};

#endif //TURRETMANAGER_H
