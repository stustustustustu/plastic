#ifndef TURRETMANAGER_H
#define TURRETMANAGER_H

#include "../../../core/upgrades/manager/UpgradeManager.h"

#include "../Turret.h"

class TurretManager {
    private:
        std::vector<std::shared_ptr<Turret>> turrets;

        std::shared_ptr<Turret> selectedTurret;

    public:
        void update();
        void render();

        void placeTurret(TurretType type, const std::vector<float> &position);

        void openUpgradeMenu(std::shared_ptr<Turret> turret);

        void handleClick(const glm::vec2 &mousePos);
};

#endif //TURRETMANAGER_H
