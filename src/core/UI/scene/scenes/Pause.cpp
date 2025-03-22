#include "Pause.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

Pause::Pause() : Scene("PAUSE") {
    resume = std::make_unique<Button>(glm::vec2((game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2), glm::vec2(200.0f , 50.0f ), "RESUME", HEXtoRGB(0x2F2F2F));
    restart = std::make_unique<Button>(glm::vec2((game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2 + 1 * 75.0f), glm::vec2(200.0f , 50.0f ), "RESTART", HEXtoRGB(0x2F2F2F));
    settings = std::make_unique<Button>(glm::vec2((game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2 + 2 * 75.0f), glm::vec2(200.0f , 50.0f ), "SETTINGS", HEXtoRGB(0x2F2F2F));
    exit = std::make_unique<Button>(glm::vec2((game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2 + 3 * 75.0f), glm::vec2(200.0f , 50.0f ), "SAVE & QUIT", HEXtoRGB(0x2F2F2F));

    resume -> addCallback([] {
        game -> scenes -> goBack();
        game -> setState(GameState::ACTIVE);
    });

    restart -> addCallback([] {

    });

    settings -> addCallback([] {
        game -> scenes -> switchScene("SETTINGS");
        game -> setState(GameState::ACTIVE);
    });

    exit -> addCallback([] {
        if (game -> getCurrentWorld()) {
            std::string name = game -> getCurrentWorld() -> getName();
            game -> getCurrentWorld() -> save(name);
        }
        game -> scenes -> switchScene("MAIN_MENU");
        game -> scenes -> clearHistory();
    });
}

void Pause::render() {
    // darken
    game -> renderer -> DrawSpriteSheet(*game -> texture, glm::vec2(0, 0), 2, 32, 32, glm::vec2(game -> getSize().x, game -> getSize().y), 0, HEXtoRGB(0x000000), 0.25);

    game -> renderer -> DrawText("GAME PAUSED", {(game -> getSize().x - game -> text -> GetWidth("GAME PAUSED", 48)) / 2, (game -> getSize().y - 275.0f) / 2 - 40}, 48.0f, true);

    resume -> render();
    restart -> render();
    settings -> render();
    exit -> render();
}

void Pause::update() {
    resume -> update();
    restart -> update();
    settings -> update();
    exit -> update();
}

void Pause::resize() {
    resume -> setPosition({(game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2});
    restart -> setPosition({(game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2 + 1 * 75.0f});
    settings -> setPosition({(game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2 + 2 * 75.0f});
    exit -> setPosition({(game -> getSize().x - 200.0f ) / 2, (game -> getSize().y - 275.0f) / 2 + 3 * 75.0f});
}