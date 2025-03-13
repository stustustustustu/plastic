#include "WorldSelection.h"

#include "../src/Game.h"

#include "WorldCreation.h"

const auto game = Game::getInstance();

WorldSelection::WorldSelection() : Scene("WORLD_SELECTION") {
    backButton = std::make_unique<Button>(glm::vec2(game -> getSize().at(0), game -> getSize().at(1)) - glm::vec2(175, 75), glm::vec2(150, 50), "Cancel", HEXtoRGB(0xFF4444));
    createWorldButton = std::make_unique<Button>(glm::vec2(25, game -> getSize().at(1)) - glm::vec2(0, 75), glm::vec2(200, 50), "New World", HEXtoRGB(0x4444FF));

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
    waveIndices.clear();

    for (const auto& entry : std::filesystem::directory_iterator("saves")) {
        if (entry.path().extension() == ".bin") {
            std::ifstream file(entry.path(), std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << entry.path() << std::endl;
                continue;
            }

            size_t nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);

            Difficulty difficulty;
            int waveIndex;
            int seed;

            file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
            file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
            file.read(reinterpret_cast<char*>(&waveIndex), sizeof(waveIndex));

            std::string diff;
            switch (static_cast<int>(difficulty)) {
                case 1:
                    diff = "EASY";
                    break;
                case 2:
                    diff = "MEDIUM";
                    break;
                case 3:
                    diff = "HARD";
                    break;
                case 4:
                    diff = "EXPERT";
                    break;
                case 5:
                    diff = "IMPOSSIBLE";
                    break;
                default:
                    diff = "MEDIUM";
            }

            worldNames.push_back(name);
            worldDifficulties.push_back(diff);
            waveIndices.push_back(waveIndex);

            std::cout << "Loaded world: " << name << ", Difficulty: " << diff << ", Wave: " << waveIndex << std::endl;

            file.close();
        }
    }
}

void WorldSelection::createWorldButtons() {
    playButtons.clear();
    deleteButtons.clear();

    for (size_t i = 0; i < worldNames.size(); ++i) {
        glm::vec2 position(275 - 48, 25 + i * (100 + border));
        playButtons.push_back(std::make_unique<Button>(position + glm::vec2(-border / 2, border / 2), glm::vec2(48, 24), "PLAY", HEXtoRGB(0x3F3F3F)));
        deleteButtons.push_back(std::make_unique<Button>(position + glm::vec2(24 - border / 2, 100 - 24 - border / 2), glm::vec2(24, 24), "", HEXtoRGB(0xFF4444)));

        playButtons.back() -> addCallback([this, i]() {
            game -> loadWorld("saves/" + worldNames[i] + ".bin");
            game -> scenes -> switchScene("IN_GAME");
        });

        deleteButtons.back() -> addCallback([this, i]() {
            std::filesystem::remove("saves/" + worldNames[i] + ".bin");
            loadWorlds();
            createWorldButtons();
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

        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(27, 27 + i * (100 + border)), 2, 32, 32, glm::vec2(size + 48 + border, 100), 0, HEXtoRGB(0x000000));

        game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(25, 25 + i * (100 + border)), 2, 32, 32, glm::vec2(size + 48 + border, 100), 0, HEXtoRGB(0x2F2F2F));

        playButtons[i] -> setPosition(glm::vec2(size + 24 + border, 25 + i * (100 + border)) + glm::vec2(-border / 2, border / 2));
        deleteButtons[i] -> setPosition(glm::vec2(size + 24 + border, 25 + i * (100 + border)) + glm::vec2(24 - border / 2, 100 - 24 - border / 2));

        game -> renderer -> DrawText(worldNames[i], glm::vec2(25 + border / 2, 50 + i * (100 + border)), 24.0f, true);
        game -> renderer -> DrawText("Difficulty: " + worldDifficulties[i], glm::vec2(25 + border / 2, 80 + i * (100 + border)), 16.0f, true);
        game -> renderer -> DrawText("Wave: " + std::to_string(waveIndices[i]), glm::vec2(25 + border / 2, 100 + i * (100 + border)), 16.0f, true);
        game -> renderer -> DrawText("Window size: " + std::to_string(static_cast<int>(game -> getSize().at(0))) + "x" + std::to_string(static_cast<int>(game -> getSize().at(1))), glm::vec2(25 + border / 2, 120 + i * (100 + border)), 16.0f, true);

        playButtons[i] -> render();
        deleteButtons[i] -> render();
    }
}

void WorldSelection::update() {
    createWorldButton -> update();
    backButton -> update();

    for (auto& button : playButtons) {
        button -> update();
    }
    for (auto& button : deleteButtons) {
        button -> update();
    }
}

void WorldSelection::resize() {
    createWorldButtons();
}