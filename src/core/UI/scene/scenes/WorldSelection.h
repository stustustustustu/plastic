#ifndef WORLDSELECTION_H
#define WORLDSELECTION_H

#include "../Scene.h"
#include "../../elements/button/Button.h"
#include "../src/core/world/World.h"

class WorldSelection : public Scene {
    private:
        float border = 8;

    private:
        std::vector<std::string> worldNames;
        std::vector<std::string> worldDifficulties;
        std::vector<int> waveIndices;
        std::vector<std::unique_ptr<Button>> playButtons;
        std::vector<std::unique_ptr<Button>> deleteButtons;

        std::unique_ptr<Button> createWorldButton;
        std::unique_ptr<Button> backButton;

        void loadWorlds();
        void createWorldButtons();

    public:
        WorldSelection();
        void render() override;
        void update() override;
        void resize() override;


};

#endif //WORLDSELECTION_H
