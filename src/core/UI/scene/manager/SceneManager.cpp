#include "SceneManager.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

void SceneManager::initScene() const {
    if (!currentScene) return;

    currentScene -> setElementAction(0, []() {
        std::cout << "Button clicked!" << std::endl;
    });
}

void SceneManager::addScene(const std::string &sceneID, std::unique_ptr<Scene> scene) {
    scenes[sceneID] = std::move(scene);
}

void SceneManager::switchScene(const std::string &sceneID) {
    if (scenes.find(sceneID) != scenes.end()) {
        if (currentScene && currentScene -> getSceneID() != "PAUSE") {
            sceneHistory.push(currentScene -> getSceneID());
        }
        currentScene = scenes[sceneID].get();
        currentScene -> resize();
    }
}

void SceneManager::goBack() {
    if (!sceneHistory.empty()) {
        std::string previousSceneID = sceneHistory.top();
        sceneHistory.pop();

        currentScene = scenes[previousSceneID].get();
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

std::string SceneManager::getScene() const {
    return currentScene ? currentScene -> getSceneID() : "";
}

Scene* SceneManager::getScene(const std::string& name) const {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        return it->second.get();
    }
    return nullptr;
}

void SceneManager::clearHistory() {
    sceneHistory = {};
}
