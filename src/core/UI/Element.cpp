#include "Element.h"

void Element::setPosition(glm::vec2 position) {
    this -> position = position;
}

glm::vec2 Element::getPosition() const {
    return position;
}

void Element::setSize(glm::vec2 size) {
    this -> size = size;
}

glm::vec2 Element::getSize() const {
    return size;
}
