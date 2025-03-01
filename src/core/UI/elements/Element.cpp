#include "Element.h"
#include "../../../Game.h"

const auto game = Game::getInstance();

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

bool Element::isHovering() const {
    double mouseX, mouseY;
    glfwGetCursorPos(game -> window, &mouseX, &mouseY);
    return (mouseX >= position.x && mouseX <= (position + size).x) && (mouseY >= position.y && mouseY <= (position + size).y);
}

bool Element::isClicked() const {
    return isHovering() && glfwGetMouseButton(game -> window, 0) == GLFW_PRESS;
}

bool Element::debounce(float debounce) {
    auto now = std::chrono::high_resolution_clock::now();
    float timeSinceLastAction = std::chrono::duration<float>(now - lastActionTime).count();

    if (timeSinceLastAction >= debounce) {
        lastActionTime = now;
        return true;
    }
    return false;
}

void Element::addCallback(const Action::Callback &callback) {
    action.addCallback(callback);
}

void Element::executeAction() {
    action.execute();
}
