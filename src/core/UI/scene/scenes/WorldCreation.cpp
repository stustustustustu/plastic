#include "WorldCreation.h"

#include "../../../../Game.h"
#include "../src/core/UI/scene/manager/SceneManager.h"
#include "../src/core/UI/scene/scenes/MainMenu.h"

auto const game = Game::getInstance();

WorldCreation::WorldCreation() : Scene("WORLD_CREATION"), islandPreview(0) {
    backButton = std::make_unique<Button>(glm::vec2(game -> getSize().at(0), game -> getSize().at(1)) - glm::vec2(175, 75), glm::vec2(150, 50), "Cancel", HEXtoRGB(0xFF4444));

    seedInput = std::make_unique<Textbox>(glm::vec2(25, 25), glm::vec2(200, 50), "Seed");
    generateSeedButton = std::make_unique<Button>(glm::vec2(235, 25), glm::vec2(game -> text -> GetWidth("Reroll", 16.0f) + 50, 50), "Reroll", HEXtoRGB(0x3F3F3F));

    difficultyDropdown = std::make_unique<Dropdown>(glm::vec2(25, 85), glm::vec2(200, 50), "Difficulty");
    difficultyDropdown -> addOption("Easy");
    difficultyDropdown -> addOption("Medium");
    difficultyDropdown -> addOption("Hard");
    difficultyDropdown -> addOption("Expert");
    difficultyDropdown -> addOption("Impossible");

    createWorldButton = std::make_unique<Button>(glm::vec2(25, game -> getSize().at(1)) - glm::vec2(0, 75), glm::vec2(200, 50), "Create World", HEXtoRGB(0x4444FF));

    currentSeed = std::pow(rand(), 2);
    islandPreview = Island(currentSeed);

    seedInput -> setText(std::to_string(currentSeed));

    backButton -> addCallback([]() {
        game -> scenes -> goBack();
    });

    generateSeedButton -> addCallback([this]() {
        currentSeed = std::pow(rand(), 2);
        seedInput -> setText(std::to_string(currentSeed));
        updateIslandPreview();
    });

    seedInput -> addCallback([this]() {
        std::string input = seedInput -> getText();
        unsigned int seed = 0;

        for (const char ch : input) {
            if (ch >= '0' && ch <= '9') {
                seed = (seed * 10) + (ch - '0');
            } else {
                seed = (seed * 31) + static_cast<unsigned int>(ch);
            }
        }

        currentSeed = seed % (INT_MAX + 1);

        updateIslandPreview();
    });

    createWorldButton -> addCallback([this]() {
        game -> createNewWorld(currentSeed, static_cast<Difficulty>(difficultyDropdown -> getSelectedIndex()));

        updateIslandPreview();

        game -> getCurrentWorld() -> player -> setPosition({static_cast<float>(game -> getCurrentWorld() -> player -> calculateSpawnTile().first * Island::TILE_SIZE + Island::TILE_SIZE / 2),
                                                               static_cast<float>(game -> getCurrentWorld() -> player -> calculateSpawnTile().second) * Island::TILE_SIZE + Island::TILE_SIZE / 2});

        game -> scenes -> switchScene("IN_GAME");
    });
}

void WorldCreation::resize() {
    backButton -> setPosition(glm::vec2(game -> getSize().at(0), game -> getSize().at(1)) - glm::vec2(175, 75));
    createWorldButton -> setPosition(glm::vec2(25, game -> getSize().at(1)) - glm::vec2(0, 75));

    updateIslandPreview();
}

void WorldCreation::render() {
    islandPreview.render(*game -> texture);

    backButton -> render();
    generateSeedButton -> render();
    createWorldButton -> render();

    difficultyDropdown -> render();
    seedInput -> render();
}

void WorldCreation::update() {
    backButton -> update();
    generateSeedButton -> update();
    createWorldButton -> update();

    difficultyDropdown -> update();
    seedInput -> update();
}

void WorldCreation::updateIslandPreview() {
    islandPreview = Island(currentSeed);
}