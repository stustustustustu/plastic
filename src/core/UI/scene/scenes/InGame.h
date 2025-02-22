#ifndef INGAME_H
#define INGAME_H

#include "../Scene.h"

class InGame : public Scene {
    private:
        bool isHoveringHealth = false;
        bool isHoveringShield = false;

        int border = 8;
        int portrait = 48;
        int width = 100;

    public:
        InGame() : Scene(SceneType::GAME) {}
        void render() override;
        void update() override;

    private:
        void renderPlayerStats();
        void renderWaveInfo();
        void renderPopup(const std::string& text, const glm::vec2& position, const glm::vec3& color);
};

#endif //INGAME_H
