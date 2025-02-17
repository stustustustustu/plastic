#include "SceneManager.h"

#include "../../UI/elements/button/Button.h"

void SceneManager::initScene() {
    if (!currentScene) return;
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