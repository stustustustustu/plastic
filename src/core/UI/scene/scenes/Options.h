#ifndef OPTIONS_H
#define OPTIONS_H

#include "../Scene.h"

#include "../../elements/button/Button.h"
#include "../../elements/dropdown/Dropdown.h"
#include "../../elements/slider/Slider.h"

class Options : public Scene {
    private:
        std::unique_ptr<Button> backButton;
        std::unique_ptr<Button> saveButton;

        std::unique_ptr<Slider> volumeSlider;

        std::unique_ptr<Dropdown> sizeDropdown;

    public:
        Options();
        void render() override;
        void update() override;

        void resize() override;
};

#endif //OPTIONS_H
