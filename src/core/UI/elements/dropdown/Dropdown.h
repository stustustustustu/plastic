#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "../src/config.h"
#include "../../Element.h"

class Dropdown : public Element {
    private:
        bool open = false;
        std::vector<Element*> elements;

    public:
        Dropdown(glm::vec2 position, glm::vec2 size) : Element(position, size) {}

        void render() override;
        void update() override;

        void addElement(Element* element);
};



#endif //DROPDOWN_H
