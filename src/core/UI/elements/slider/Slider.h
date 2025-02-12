#ifndef SLIDER_H
#define SLIDER_H

#include "../src/config.h"
#include "../../Element.h"

class Slider : public Element {
    private:
        float min;
        float max;
        float value;

        float rounding;
        bool dragging = false;

    public:
        Slider(glm::vec2 position, glm::vec2 size, float min, float max, float value, float rounding) : Element(position, size), min(min), max(max), value(value), rounding(rounding) {}

        void render() override;
        void update() override;
};

#endif //SLIDER_H
