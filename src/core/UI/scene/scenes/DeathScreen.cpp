#include "DeathScreen.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

DeathScreen::DeathScreen() : Scene("DEATH_SCREEN") {
    restartButton = std::make_unique<Button>(glm::vec2(game -> getSize().x - 200 - 25, 50 + 1 * 25), glm::vec2(200, 50), "RESTART", HEXtoRGB(0x2F2F2F));
    mainMenuButton = std::make_unique<Button>(glm::vec2(game -> getSize().x - 200 - 25, 100 + 2 * 25), glm::vec2(200, 50), "MAIN MENU", HEXtoRGB(0x2F2F2F));
    lastSaveButton = std::make_unique<Button>(glm::vec2(game -> getSize().x - 200 - 25, 150 + 3 * 25), glm::vec2(200, 50), "LOAD LAST SAVE", HEXtoRGB(0x2F2F2F));

    if (game -> getCurrentWorld()) {
        std::filesystem::path savePath = std::filesystem::path("saves") / game -> getCurrentWorld() -> getName() / "save.bin";
        if (!std::filesystem::exists(savePath)) {
            lastSaveButton -> setActive(false);
        }
    }

    restartButton -> addCallback([this]() {
        if (game -> getCurrentWorld()) {
            std::string name = game -> getCurrentWorld() -> getName();
            unsigned int seed = game -> getCurrentWorld() -> getSeed();
            Difficulty difficulty = game -> getCurrentWorld() -> getDifficulty();

            game -> createNewWorld(name, seed, difficulty);
            game -> getCurrentWorld() -> player -> setPosition({static_cast<float>(game -> getCurrentWorld() -> player -> calculateSpawnTile().first * Island::TILE_SIZE + Island::TILE_SIZE / 2),
                                                       static_cast<float>(game -> getCurrentWorld() -> player -> calculateSpawnTile().second) * Island::TILE_SIZE + Island::TILE_SIZE / 2});
            game -> scenes -> switchScene("IN_GAME");
        }
    });

    mainMenuButton -> addCallback([]() {
        game -> scenes -> switchScene("MAIN_MENU");
    });

    lastSaveButton -> addCallback([this]() {
    if (game -> getCurrentWorld()) {
        std::string name = game -> getCurrentWorld() -> getName();
        game -> loadWorld(name);
        game -> scenes -> switchScene("IN_GAME");
    }
});
}

void DeathScreen::render() {
    game -> renderer -> DrawText("YOU LOST", glm::vec2((game -> getSize().x - game -> text -> GetWidth("YOU LOST", 48.0f)) / 2, game -> getSize().y / 2 - 48), 48.0f, true);

    restartButton -> render();
    mainMenuButton -> render();
    lastSaveButton -> render();
}

void DeathScreen::update() {
    restartButton -> update();
    mainMenuButton -> update();
    lastSaveButton -> update();
}

void DeathScreen::resize() {
    std::cout << "player died" << std::endl;

    restartButton -> setPosition(glm::vec2(game -> getSize().x - 200 - 25, 50 + 1 * 25));
    mainMenuButton -> setPosition(glm::vec2(game -> getSize().x - 200 - 25, 100 + 2 * 25));
    lastSaveButton -> setPosition(glm::vec2(game -> getSize().x - 200 - 25, 150 + 3 * 25));
}