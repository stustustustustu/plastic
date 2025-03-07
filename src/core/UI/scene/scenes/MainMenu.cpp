#include "MainMenu.h"

#include "../../../../Game.h"
#include "../src/core/UI/scene/scenes/InGame.h"

const auto game = Game::getInstance();

MainMenu::MainMenu() : Scene(SceneType::MENU) {
    playButton = std::make_unique<Button>(glm::vec2(border, 50 + 1 * border), glm::vec2(200, 50), "PLAY", HEXtoRGB(0x2F2F2F));
    optionsButton = std::make_unique<Button>(glm::vec2(border, 100 + 2 * border), glm::vec2(200, 50), "SETTINGS", HEXtoRGB(0x2F2F2F));
    quitButton = std::make_unique<Button>(glm::vec2(border, 150 + 3 * border), glm::vec2(200, 50), "QUIT", HEXtoRGB(0x2F2F2F));

    playButton -> addCallback([]() {
        game -> scenes -> switchScene(SceneType::WORLD_CREATION);
    });

    optionsButton -> addCallback([]() {
        game -> scenes -> switchScene(SceneType::SETTINGS);
    });

    quitButton -> addCallback([]() {
        glfwSetWindowShouldClose(game -> window, true);
    });

    addElement(std::move(playButton));
    addElement(std::move(optionsButton));
    addElement(std::move(quitButton));
}

void MainMenu::resize() {}

void MainMenu::render() {
    for (const auto& element : getElements()) {
        element -> render();
    }
}

void MainMenu::update() {
    for (const auto& element : getElements()) {
        element -> update();
    }
}