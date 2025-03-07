#ifndef MAINMENU_H
#define MAINMENU_H

#include "../Scene.h"

#include "../../elements/button/Button.h"

class MainMenu : public Scene {
    private:
        std::unique_ptr<Button> playButton;
        std::unique_ptr<Button> optionsButton;
        std::unique_ptr<Button> quitButton;

    public:
        MainMenu();

        void render() override;
        void update() override;

        void resize() override;
};

#endif //MAINMENU_H
