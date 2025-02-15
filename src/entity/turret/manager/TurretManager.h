#ifndef TURRETMANAGER_H
#define TURRETMANAGER_H

#include "../../../core/upgrades/manager/UpgradeManager.h"

#include "../Turret.h"
#include "shop/Shop.h"
#include "Upgrades.h"

class TurretManager {
    private:
        std::vector<std::shared_ptr<Turret>> turrets;

        bool showShop = false;
        bool showUpgrades = false;

        std::shared_ptr<Turret> selectedTurret;

    private:
        TurretShop *shop = new TurretShop(glm::vec2(5, 55), glm::vec2(170, 500));

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
