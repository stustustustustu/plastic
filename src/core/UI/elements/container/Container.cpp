#include "Container.h"

void Container::render() {
    for (auto &element : elements) {
        element -> render();
    }
}

void Container::update() {
    for (auto &element : elements) {
        element -> update();
    }
}

void Container::addElement(Element *element) {
    elements.push_back(element);
}
