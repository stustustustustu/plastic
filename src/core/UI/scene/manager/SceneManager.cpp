#include "SceneManager.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

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
        currentScene -> resize();
    }
}

void SceneManager::render() {
    game -> renderer -> SetProjection(game -> camera -> getStaticProjection());

    if (currentScene) {
        currentScene -> render();
    }
}

void SceneManager::update() {
    if (currentScene) {
        currentScene -> update();
    }
}

void SceneManager::resize() {
    if (currentScene) {
        currentScene -> resize();
    }
}


SceneType SceneManager::getScene() const {
    return currentScene ? currentScene -> getType() : SceneType::NONE;
}