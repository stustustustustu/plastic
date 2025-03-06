#ifndef WORLDCREATION_H
#define WORLDCREATION_H

#include "../Scene.h"
#include "../../../island/Island.h"

#include "../../elements/button/Button.h"
#include "../../elements/dropdown/Dropdown.h"
#include "../../elements/textbox/Textbox.h"

class WorldCreation : public Scene {
    private:
        unsigned int currentSeed;

    private:
        std::unique_ptr<Button> backButton;
        std::unique_ptr<Button> generateSeedButton;
        std::unique_ptr<Button> createWorldButton;
        std::unique_ptr<Textbox> worldNameInput;
        std::unique_ptr<Textbox> seedInput;
        std::unique_ptr<Dropdown> difficultyDropdown;

        Island islandPreview;

    public:
        WorldCreation();
        void render() override;
        void update() override;

        void resize() override;

        void updateIslandPreview();
};

#endif //WORLDCREATION_H
