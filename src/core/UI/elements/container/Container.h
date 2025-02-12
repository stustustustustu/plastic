#ifndef CONTAINER_H
#define CONTAINER_H

#include "../src/config.h"
#include "../../Element.h"

class Container : public Element {
    private:
        std::vector<Element*> elements;

    public:
        Container(glm::vec2 position, glm::vec2 size) : Element(position, size) {}

        void render() override;
        void update() override;

        void addElement(Element* element);
};

#endif //CONTAINER_H
