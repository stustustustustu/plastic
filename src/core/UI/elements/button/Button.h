#ifndef BUTTON_H
#define BUTTON_H

#include "../src/config.h"
#include "../Element.h"

class Button : public Element {
    private:
        std::string label;
        glm::vec3 color;
        bool hovered = false;
        bool clicked = false;

    public:
        Button(glm::vec2 position, glm::vec2 size, const std::string& label, glm::vec3 color) : Element(position, size), label(label), color(color) {}

        void render() override;
        void update() override;

        void setLabel(const std::string &label) { this -> label = label; }
};

#endif //BUTTON_H