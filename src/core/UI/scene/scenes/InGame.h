#ifndef INGAME_H
#define INGAME_H

#include "../Scene.h"
#include "../../../upgrades/panel/UpgradePanel.h"
#include "../../elements/button/Button.h"
#include "../../elements/toggle/Toggle.h"

class InGame : public Scene {
    private:
        bool isHoveringHealth = false;
        bool isHoveringShield = false;
        bool isShopOpen = true;
        bool isAdvancedView = false;

        float border = 8;
        float portrait = 48;
        int width = 100;

    std::vector<Button> buttons;

    std::vector<std::unique_ptr<UpgradePanel>> upgradePanels;
    std::vector<Toggle> toggles;

    public:
        InGame();
        void render() override;
        void update() override;

    private:
        void renderPlayerStats();
        void renderAdvancedStats();
        void renderPlayerShop();

        void renderWaveInfo();
        void renderPopup(const std::string& text, const glm::vec2& position, const glm::vec3& color);

    public:
        void refreshUpgradePanels();

};

#endif //INGAME_H
