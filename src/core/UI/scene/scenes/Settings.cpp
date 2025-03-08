#include "Settings.h"

#include "../../../../Game.h"
#include "../src/core/UI/scene/manager/SceneManager.h"
#include "../src/core/UI/scene/scenes/MainMenu.h"

const auto game = Game::getInstance();

Settings::Settings() : Scene("SETTINGS") {
    backButton = std::make_unique<Button>(glm::vec2(game -> getSize().at(0), game -> getSize().at(1)) - glm::vec2(175, 75), glm::vec2(150, 50), "Cancel", HEXtoRGB(0xFF4444));
    saveButton = std::make_unique<Button>(glm::vec2(25, game -> getSize().at(1)) - glm::vec2(0, 75), glm::vec2(200, 50), "Save", HEXtoRGB(0x4444FF));

    volumeSlider = std::make_unique<Slider>(glm::vec2(25, 25), glm::vec2(200, 20), 0.0f, 1.0f, 0.5f, false);

    sizeDropdown = std::make_unique<Dropdown>(glm::vec2(25, 70), glm::vec2(200, 50), "Window Size");
    sizeDropdown -> addOption("640x480");
    sizeDropdown -> addOption("800x600");
    sizeDropdown -> addOption("1024x768");
    sizeDropdown -> addOption("1280x960");

    backButton -> addCallback([]() {
        game -> scenes -> goBack();
    });

    saveButton -> addCallback([this]() {
        float volume = volumeSlider -> getValue();
        game -> setVolume(volume);

        std::string size = sizeDropdown -> getSelectedOption();
        game -> setSize(size);

        game -> scenes -> goBack();
    });
}

void Settings::resize() {
    saveButton -> setPosition(glm::vec2(25, game -> getSize().at(1)) - glm::vec2(0, 75));
    backButton -> setPosition(glm::vec2(game -> getSize().at(0), game -> getSize().at(1)) - glm::vec2(175, 75));
}

void Settings::render() {
    saveButton -> render();
    backButton -> render();

    sizeDropdown -> render();

    volumeSlider -> render();
}

void Settings::update() {
    saveButton -> update();
    backButton -> update();

    sizeDropdown -> update();

    volumeSlider -> update();
}
