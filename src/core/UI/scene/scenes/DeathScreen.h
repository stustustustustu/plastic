#ifndef DEATHSCREEN_H
#define DEATHSCREEN_H

#include "../Scene.h"
#include "../../elements/button/Button.h"

class DeathScreen : public Scene {
    private:
        std::unique_ptr<Button> restartButton;
        std::unique_ptr<Button> mainMenuButton;
        std::unique_ptr<Button> lastSaveButton;

    public:
        DeathScreen();

        void render() override;
        void update() override;
        void resize() override;
};

#endif //DEATHSCREEN_H
