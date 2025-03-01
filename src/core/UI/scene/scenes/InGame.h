#ifndef INGAME_H
#define INGAME_H

#include "../Scene.h"
#include "../../../../entity/turret/panel/ShopPanel.h"
#include "../../../upgrades/panel/UpgradePanel.h"
#include "../../elements/button/Button.h"
#include "../../elements/toggle/Toggle.h"

class InGame : public Scene {
    private:
        bool isHoveringHealth = false;
        bool isHoveringShield = false;
        bool isPlayerShopOpen = false;
        bool isTurretShopOpen = false;
        bool isAdvancedView = false;

        float border = 8;
        float portrait = 48;
        int width = 100;

        Button portraitToggle;
        Button playerShopToggle;
        Button turretShopToggle;

        std::vector<std::unique_ptr<UpgradePanel>> upgradePanels;
        std::vector<std::unique_ptr<ShopPanel>> shopPanels;

    public:
        InGame();
        void render() override;
        void update() override;

    private:
        void renderPlayerStats();
        void renderAdvancedStats();

        void renderPlayerShop();

        void renderTurretShop();

        void renderWaveInfo();
        void renderPopup(const std::string& text, const glm::vec2& position, const glm::vec3& color);

    public:
        void refreshUpgradePanels();

};

#endif //INGAME_H
