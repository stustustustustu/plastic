#include "WorldSelection.h"

#include "../src/Game.h"

#include "WorldCreation.h"

const auto game = Game::getInstance();

WorldSelection::WorldSelection() : Scene("WORLD_SELECTION") {
    backButton = std::make_unique<Button>(game -> getSize() - glm::vec2(175, 75), glm::vec2(150, 50), "Cancel", HEXtoRGB(0xFF4444));
    createWorldButton = std::make_unique<Button>(glm::vec2(25, game -> getSize().y) - glm::vec2(0, 75), glm::vec2(200, 50), "New World", HEXtoRGB(0x4444FF));

    backButton -> addCallback([]() {
        game -> scenes -> goBack();
    });

    createWorldButton -> addCallback([]() {
        game -> scenes -> switchScene("WORLD_CREATION");
    });

    loadWorlds();
    createWorldButtons();
}

void WorldSelection::loadWorlds() {
    worldNames.clear();
    worldDifficulties.clear();
    worldScores.clear();
    waveIndices.clear();

    for (const auto& entry : std::filesystem::directory_iterator("saves")) {
        if (entry.is_directory()) {
            std::string worldName = entry.path().filename().string();

            std::filesystem::path savePath = entry.path() / "save.bin";

            if (!std::filesystem::exists(savePath)) {
                std::cerr << "Save file not found for world: " << worldName << std::endl;
                continue;
            }

            std::ifstream file(savePath, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Failed to open save file for world: " << worldName << std::endl;
                continue;
            }

            size_t nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);

            int seed;
            Difficulty difficulty;
            unsigned int score;
            int waveIndex;

            file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
            file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
            file.read(reinterpret_cast<char*>(&score), sizeof(score));
            file.read(reinterpret_cast<char*>(&waveIndex), sizeof(waveIndex));

            std::string diff;
            switch (static_cast<int>(difficulty)) {
                case 0:
                    diff = "EASY";
                    break;
                case 1:
                    diff = "MEDIUM";
                    break;
                case 2:
                    diff = "HARD";
                    break;
                case 3:
                    diff = "EXPERT";
                    break;
                case 4:
                    diff = "IMPOSSIBLE";
                    break;
                default:
                    diff = "MEDIUM";
            }

            worldNames.push_back(name);
            worldDifficulties.push_back(diff);
            worldScores.push_back(score);
            waveIndices.push_back(waveIndex);

            std::cout << "Loaded world: " << name << ", Difficulty: " << diff << ", Score: " << score << ", Wave: " << waveIndex << std::endl;

            file.close();
        }
    }
}

void WorldSelection::createWorldButtons() {
    playButtons.clear();
    deleteButtons.clear();

    for (size_t i = 0; i < worldNames.size(); ++i) {
        auto size = std::max(200.0f, game -> text -> GetWidth(worldNames[i], 24) + border);

        glm::vec2 position(size + 24 + border, 25 + i * (100 + border));
        playButtons.push_back(std::make_unique<Button>(position + glm::vec2(-border / 2, border / 2), glm::vec2(48, 24), "PLAY", HEXtoRGB(0x3F3F3F)));
        replayButtons.push_back(std::make_unique<Button>(position + glm::vec2(-border / 2 - 20, border + 24), glm::vec2(68, 24), "REPLAY", HEXtoRGB(0x3F3F3F)));
        deleteButtons.push_back(std::make_unique<Button>(position + glm::vec2(24 - border / 2, 120 - 24 - border / 2), glm::vec2(24, 24), "", HEXtoRGB(0xFF4444)));

        playButtons.back() -> addCallback([this, i]() {
            game -> loadWorld(worldNames[i]);
            game -> scenes -> switchScene("IN_GAME");
        });

        replayButtons.back() -> addCallback([this, i] () {
            if (game -> replay) {
                game -> scenes -> switchScene("IN_REPLAY");
                game -> replay -> loadReplay("saves/" + worldNames[i] + "/replay.bin");
                game -> replay -> setPlaying(true);
            }
        });

        deleteButtons.back() -> addCallback([this, i]() {
            try {
                    std::string worldPath = "saves/" + worldNames[i];
                    if (std::filesystem::exists(worldPath)) {
                        std::filesystem::remove_all(worldPath);
                        loadWorlds();
                        createWorldButtons();
                    } else {
                        std::cerr << "World directory does not exist: " << worldPath << std::endl;
                    }
                } catch (const std::filesystem::filesystem_error& e) {
                    std::cerr << "Failed to delete world: " << e.what() << std::endl;
                }
        });
    }

    if (worldNames.empty()) {
        game -> scenes -> switchScene("WORLD_CREATION");
    }
}

void WorldSelection::render() {
    createWorldButton -> render();
    backButton -> render();

    for (size_t i = 0; i < worldNames.size(); ++i) {
        auto size = std::max(200.0f, game -> text -> GetWidth(worldNames[i], 24) + border);

        game -> renderer -> DrawRect(glm::vec2(27, 27 + i * (100 + border)), glm::vec2(size + 48 + border, 120), 0, HEXtoRGB(0x000000));

        game -> renderer -> DrawRect(glm::vec2(25, 25 + i * (100 + border)), glm::vec2(size + 48 + border, 120), 0, HEXtoRGB(0x2F2F2F));

        game -> renderer -> DrawText(worldNames[i], glm::vec2(25 + border / 2, 50 + i * (100 + border)), 24.0f, true);
        game -> renderer -> DrawText("Difficulty: " + worldDifficulties[i], glm::vec2(25 + border / 2, 80 + i * (100 + border)), 16.0f, true);
        game -> renderer -> DrawText("Score: " + std::to_string(worldScores[i]), glm::vec2(25 + border / 2, 100 + i * (100 + border)), 16.0f, true);
        game -> renderer -> DrawText("Wave: " + std::to_string(waveIndices[i]), glm::vec2(25 + border / 2, 120 + i * (100 + border)), 16.0f, true);
        game -> renderer -> DrawText("Window size: " + std::to_string(static_cast<int>(game -> getSize().x)) + "x" + std::to_string(static_cast<int>(game -> getSize().y)), glm::vec2(25 + border / 2, 140 + i * (100 + border)), 16.0f, true);

        playButtons[i] -> render();
        replayButtons[i] -> render();
        deleteButtons[i] -> render();
    }
}

void WorldSelection::update() {
    createWorldButton -> update();
    backButton -> update();

    for (auto& button : playButtons) {
        button -> update();
    }
    for (auto& button : replayButtons) {
        button -> update();
    }
    for (auto& button : deleteButtons) {
        button -> update();
    }
}

void WorldSelection::resize() {
    createWorldButtons();

    backButton -> setPosition(game -> getSize() - glm::vec2(175, 75));
    createWorldButton -> setPosition(glm::vec2(25, game -> getSize().y) - glm::vec2(0, 75));
}