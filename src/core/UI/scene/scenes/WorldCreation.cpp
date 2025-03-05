#include "WorldCreation.h"

#include "../../../../Game.h"
#include "../src/core/UI/scene/manager/SceneManager.h"
#include "../src/core/UI/scene/scenes/MainMenu.h"

auto const game = Game::getInstance();

WorldCreation::WorldCreation() : Scene(SceneType::WORLD_CREATION), islandPreview(0) {
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
        game -> scenes -> switchScene(SceneType::MENU);
    });

    generateSeedButton -> addCallback([this]() {
        currentSeed = std::pow(rand(), 2);
        seedInput -> setText(std::to_string(currentSeed));
        updateIslandPreview();
    });

    createWorldButton -> addCallback([this]() {
        game -> setDifficulty(static_cast<Difficulty>(difficultyDropdown -> getSelectedIndex()));
        game -> setSeed(std::stoul(seedInput -> getText()));

        updateIslandPreview();

        game -> scenes -> switchScene(SceneType::GAME);
        game -> player -> setPosition({static_cast<float>(game -> player -> calculateSpawnTile().first * Island::TILE_SIZE + Island::TILE_SIZE / 2),
                                          static_cast<float>(game -> player -> calculateSpawnTile().second) * Island::TILE_SIZE + Island::TILE_SIZE / 2});
    });

    addElement(std::move(backButton));
    addElement(std::move(generateSeedButton));
    addElement(std::move(createWorldButton));
}

void WorldCreation::updateIslandPreview() {
    islandPreview = Island(currentSeed);
    game -> generator = &islandPreview;
}

void WorldCreation::render() {
    game -> generator -> render(*game -> texture);

    for (const auto& element : getElements()) {
        element -> render();
    }

    difficultyDropdown -> render();
    seedInput -> render();
}

void WorldCreation::update() {
    for (const auto& element : getElements()) {
        element -> update();
    }

    difficultyDropdown -> update();
    seedInput -> update();
}