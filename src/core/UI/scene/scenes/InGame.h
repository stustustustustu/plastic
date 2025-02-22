#ifndef INGAME_H
#define INGAME_H

#include "../Scene.h"

class InGame : public Scene {
    public:
        InGame() : Scene(SceneType::GAME) {}
        void render() override;
        void update() override;

    private:
        void renderPlayerStats();
        void renderWaveInfo();
};

#endif //INGAME_H
