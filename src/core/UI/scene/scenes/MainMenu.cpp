#include "MainMenu.h"

#include "../../../../Game.h"
#include "../src/core/UI/scene/scenes/InGame.h"

const auto game = Game::getInstance();

MainMenu::MainMenu() : Scene("MAIN_MENU") {
    playButton = std::make_unique<Button>(glm::vec2(25, 50 + 1 * 25), glm::vec2(200, 50), "PLAY", HEXtoRGB(0x2F2F2F));
    optionsButton = std::make_unique<Button>(glm::vec2(25, 100 + 2 * 25), glm::vec2(200, 50), "SETTINGS", HEXtoRGB(0x2F2F2F));
    quitButton = std::make_unique<Button>(glm::vec2(25, 150 + 3 * 25), glm::vec2(200, 50), "QUIT", HEXtoRGB(0x2F2F2F));

    playButton -> addCallback([]() {
        game -> scenes -> switchScene("WORLD_SELECTION");
    });

    optionsButton -> addCallback([]() {
        game -> scenes -> switchScene("SETTINGS");
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