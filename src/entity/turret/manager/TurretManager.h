#ifndef TURRETMANAGER_H
#define TURRETMANAGER_H

#include "../../../core/upgrades/manager/UpgradeManager.h"
#include "../Turret.h"

class TurretManager {
    private:
        std::vector<std::shared_ptr<Turret>> turrets;

        std::shared_ptr<Turret> selectedTurret;
        bool isPlacingTurret = false;
        TurretType placingTurretType = TurretType::RIFLE;

    public:
        void update();
        void render();

        void renderPreview(const glm::vec2& position) const;

        void startPlacingTurret(TurretType type);
        void placeTurret(const std::vector<float>& position);
        void cancelPlacingTurret();
        bool isPlacing() const;

        void openUpgradeMenu(std::shared_ptr<Turret> turret);

        void handleClick(const glm::vec2& mousePos);
        bool isValidPlacement(const std::vector<float>& position) const;
};

#endif // TURRETMANAGER_H