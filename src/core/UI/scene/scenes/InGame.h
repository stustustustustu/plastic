#ifndef INGAME_H
#define INGAME_H

#include "../Scene.h"
#include "../../elements/button/Button.h"

class InGame : public Scene {
    private:
        bool isHoveringHealth = false;
        bool isHoveringShield = false;
        bool isAdvancedView = false;

        float border = 8;
        float portrait = 48;
        int width = 100;

        Button portraitButton;

    public:
        InGame() : Scene(SceneType::GAME), portraitButton(glm::vec2(border, border), glm::vec2(portrait, portrait), "") {}
        void render() override;
        void update() override;

    private:
        void renderPlayerStats();
        void renderAdvancedStats();
        void renderWaveInfo();
        void renderPopup(const std::string& text, const glm::vec2& position, const glm::vec3& color);
};

#endif //INGAME_H
