#include "Dropdown.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void Dropdown::render() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, getPosition(), 2, 32, 32, size, 0, HEXtoRGB(0x323232));

    if (open) {
        glm::vec2 ePos = getPosition() + glm::vec2(0, size.y);
        for (auto &element : elements) {
            element -> setPosition(ePos);
            element -> render();
            ePos.y += element -> getSize().y + 4;
        }
    }
}

void Dropdown::update() {
    if (isClicked()) {
        toggle();
    }

    if (open) {
        for (auto& element : elements) {
            element -> update();
        }
    }
}

void Dropdown::toggle() {
    open =!open;

    if (open) {
        float totalHeight = size.y;
        for (auto &element : elements) {
            totalHeight += element -> getSize().y + 4; // 4 padding pixel
        }
        setSize({size.x, totalHeight});
    } else {
        setSize(size);
    }
}

void Dropdown::addElement(Element* element) {
    elements.push_back(element);
}