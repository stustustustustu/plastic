#ifndef BUTTON_H
#define BUTTON_H

#include "../src/config.h"
#include "../Element.h"

class Button : public Element {
    private:
        std::string label;
        bool hovered = false;
        bool clicked = false;

    public:
        Button(glm::vec2 position, glm::vec2 size, const std::string& label) : Element(position, size), label(label) {}

        void render() override;
        void update() override;
};

#endif //BUTTON_H