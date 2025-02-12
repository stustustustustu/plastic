#ifndef TOGGLE_H
#define TOGGLE_H

#include "../src/config.h"
#include "../../Element.h"

class Toggle : public Element {
    private:
        bool toggled = false;
        bool hovered = false;

    public:
        Toggle(glm::vec2 position, glm::vec2 size) : Element(position, size) {}

        void render() override;
        void update() override;
};

#endif //TOGGLE_H
