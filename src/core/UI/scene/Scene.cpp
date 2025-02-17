#include "Scene.h"

SceneType Scene::getType() const {
    return this -> type;
}

void Scene::setType(SceneType type) {
    this -> type = type;
}

void Scene::addElement(std::unique_ptr<Element> element) {
    elements.push_back(std::move(element));
}

void Scene::removeElement(const Element* element) {
    std::erase_if(elements, [element](const std::unique_ptr<Element>& e) { return e.get() == element; });
}

const std::vector<std::unique_ptr<Element>>& Scene::getElements() const {
    return elements;
}

void Scene::setElementAction(int index, const Action::Callback &callback) const {
    if (index >= 0 && index < elements.size()) {
        elements[index] -> addCallback(callback);
    }
}
