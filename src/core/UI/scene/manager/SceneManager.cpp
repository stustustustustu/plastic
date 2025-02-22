#include "SceneManager.h"

void SceneManager::initScene() const {
    if (!currentScene) return;

    currentScene -> setElementAction(0, []() {
        std::cout << "Button clicked!" << std::endl;
    });
}

void SceneManager::addScene(SceneType type, std::unique_ptr<Scene> scene) {
    scenes[type] = std::move(scene);
}

void SceneManager::switchScene(SceneType type) {
    if (scenes.find(type) != scenes.end()) {
        currentScene = scenes[type].get();
    }
}

void SceneManager::render() {
    if (currentScene) {
        currentScene -> render();
    }
}

void SceneManager::update() {
    if (currentScene) {
        currentScene -> update();
    }
}