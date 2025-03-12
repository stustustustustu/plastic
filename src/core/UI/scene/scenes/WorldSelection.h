#ifndef WORLDSELECTION_H
#define WORLDSELECTION_H

#include "../Scene.h"
#include "../../elements/button/Button.h"
#include "../src/core/world/World.h"

class WorldSelection : public Scene {
    public:
        WorldSelection();
        void render() override;
        void update() override;
        void resize() override;

    private:
        std::vector<std::string> worldNames;
        std::vector<Difficulty> worldDifficulties;
        std::vector<int> waveIndices;
        std::vector<std::unique_ptr<Button>> playButtons;
        std::vector<std::unique_ptr<Button>> deleteButtons;

        void loadWorlds();
        void createWorldButtons();
};

#endif //WORLDSELECTION_H
