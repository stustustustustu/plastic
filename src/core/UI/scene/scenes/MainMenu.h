#ifndef MAINMENU_H
#define MAINMENU_H

#include "../Scene.h"
#include "../../elements/button/Button.h"

class MainMenu : public Scene {
    private:
        float border = 8;

    private:
        std::unique_ptr<Button> playButton;
        std::unique_ptr<Button> optionsButton;
        std::unique_ptr<Button> quitButton;

    public:
        MainMenu();

        void render() override;
        void update() override;
};

#endif //MAINMENU_H
