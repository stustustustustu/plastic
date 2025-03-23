#ifndef INREPLAY_H
#define INREPLAY_H

#include "../Scene.h"

#include "../../elements/button/Button.h"
#include "../../elements/toggle/Toggle.h"
#include "../../elements/slider/Slider.h"

class InReplay : public Scene {
    private:
        float border = 8;

    private:
        std::unique_ptr<Button> speedButton;
        std::unique_ptr<Button> pauseToggle;
        std::unique_ptr<Button> debugToggle;
        std::unique_ptr<Slider> timeline;

    public:
        InReplay();

        void render() override;
        void update() override;
        void resize() override;
};

#endif //INREPLAY_H
