#ifndef INGAME_H
#define INGAME_H

#include "../Scene.h"
#include "../../elements/button/Button.h"

class InGame : public Scene {
    private:
        bool isHoveringHealth = false;
        bool isHoveringShield = false;
        bool isShopOpen = false;
        bool isAdvancedView = false;

        float border = 8;
        float portrait = 48;
        int width = 100;

        Button portraitButton;
        Button shopButton;

    public:
        InGame() : Scene(SceneType::GAME), portraitButton(glm::vec2(border, border), glm::vec2(portrait, portrait), ""), shopButton(glm::vec2(border + border / 2, portrait + (border + border / 2)), glm::vec2(portrait / 2, portrait / 2), "") {}
        void render() override;
        void update() override;

    private:
        void renderPlayerStats();
        void renderAdvancedStats();
        void renderPlayerShop();

        void renderWaveInfo();
        void renderPopup(const std::string& text, const glm::vec2& position, const glm::vec3& color);
};

#endif //INGAME_H
