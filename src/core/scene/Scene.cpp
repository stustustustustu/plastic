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
    elements.erase(std::remove_if(elements.begin(), elements.end(), [element](const std::unique_ptr<Element>& e) { return e.get() == element; }), elements.end());
}

const std::vector<std::unique_ptr<Element>>& Scene::getElements() const { return elements; }