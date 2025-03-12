#include "WorldSelection.h"

#include "../src/Game.h"

#include "WorldCreation.h"

const auto game = Game::getInstance();

WorldSelection::WorldSelection() : Scene("WORLD_SELECTION") {
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

            file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
            file.read(reinterpret_cast<char*>(&waveIndex), sizeof(waveIndex));

            worldNames.push_back(name);
            worldDifficulties.push_back(difficulty);
            waveIndices.push_back(waveIndex);

            std::cout << "Loaded world: " << name << ", Difficulty: " << static_cast<int>(difficulty) << ", Wave: " << waveIndex << std::endl;

            file.close();
        }
    }
}

void WorldSelection::createWorldButtons() {
    playButtons.clear();
    deleteButtons.clear();

    for (size_t i = 0; i < worldNames.size(); ++i) {
        glm::vec2 position(25, 50 + i * 100);
        playButtons.push_back(std::make_unique<Button>(position, glm::vec2(100, 50), "PLAY", HEXtoRGB(0x2F2F2F)));
        deleteButtons.push_back(std::make_unique<Button>(position + glm::vec2(110, 0), glm::vec2(100, 50), "DELETE", HEXtoRGB(0xFF4444)));

        playButtons.back()->addCallback([this, i]() {
            game->loadWorld("saves/" + worldNames[i] + ".bin");
            game->scenes->switchScene("IN_GAME");
        });

        deleteButtons.back()->addCallback([this, i]() {
            std::filesystem::remove("saves/" + worldNames[i] + ".bin");
            loadWorlds();
            createWorldButtons();
        });
    }

    if (worldNames.empty()) {
        game->scenes->switchScene("WORLD_CREATION");
    }
}

void WorldSelection::render() {
    for (size_t i = 0; i < worldNames.size(); ++i) {
        game->renderer->DrawText(worldNames[i], glm::vec2(25, 50 + i * 100), 24.0f, true);
        game->renderer->DrawText("Difficulty: " + std::to_string(static_cast<int>(worldDifficulties[i])), glm::vec2(25, 80 + i * 100), 16.0f, true);
        game->renderer->DrawText("Wave: " + std::to_string(waveIndices[i]), glm::vec2(25, 100 + i * 100), 16.0f, true);

        playButtons[i]->render();
        deleteButtons[i]->render();
    }
}

void WorldSelection::update() {
    for (auto& button : playButtons) {
        button->update();
    }
    for (auto& button : deleteButtons) {
        button->update();
    }
}

void WorldSelection::resize() {
    createWorldButtons();
}