#ifndef TURRETMANAGER_H
#define TURRETMANAGER_H

#include "../../../core/upgrades/manager/UpgradeManager.h"
#include "../../../core/upgrades/turret/TurretUpgradeManager.h"
#include "../Turret.h"

class TurretManager {
    private:
        std::vector<std::shared_ptr<Turret>> turrets;

        std::shared_ptr<Turret> selectedTurret;
        bool isPlacingTurret = false;
        bool isUpgradeMenuOpen = false;
        TurretType placingTurretType = TurretType::RIFLE;

    private:
        TurretUpgradeManager upgradeManager;

    public:
        glm::vec2 menuSize, menuPosition;

    public:
        TurretManager();

        void update();
        void render();

        void initUpgrades();
        std::map<Upgrade*, glm::vec2> calculateNodePositions() const;

        void renderPreview(const glm::vec2& position) const;

        void startPlacingTurret(TurretType type);
        void placeTurret(const std::vector<float>& position);
        void cancelPlacingTurret();
        bool isPlacing() const;

        void openUpgradeMenu(std::shared_ptr<Turret> turret);
        void closeUpgradeMenu();
        bool isUpgrading() const;
        bool isMouseInsideMenu(const glm::vec2 &mousePos) const;

        void handleClick(const glm::vec2& mousePos);
        bool isValidPlacement(const std::vector<float>& position) const;

        std::shared_ptr<Turret> getSelectedTurret() const;

        TurretUpgradeManager& getUpgradeManager();
        bool unlockTurretUpgrade(const std::string& upgradeName);

};

#endif // TURRETMANAGER_H