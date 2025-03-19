#include "InReplay.h"

#include "../../../../Game.h"

const auto game = Game::getInstance();

InReplay::InReplay() : Scene("IN_REPLAY") {
    speedButton = std::make_unique<Button>(glm::vec2(border, game->getSize().at(1) - 2 * (48 + border)), glm::vec2(48, 48), "1x", HEXtoRGB(0x3F3F3F));
    pauseToggle = std::make_unique<Toggle>(glm::vec2(border, game->getSize().at(1) - 48 - border), glm::vec2(48, 48));
    timeline = std::make_unique<Slider>(glm::vec2(2 * border + 48, game->getSize().at(1) - 48 - border), glm::vec2(game->getSize().at(0) - 3 * border - 48, 48), 0, 100, 0, true);

    speedButton->addCallback([this]() {
        static float speeds[] = {0.5f, 1.0f, 2.0f, 4.0f};
        static int currentSpeed = 1;

        currentSpeed = (currentSpeed + 1) % 4;

        game->replay->setPlaybackSpeed(speeds[currentSpeed]);
    });
    
    pauseToggle->addCallback([this]() {
        if (game->replay->isPlaying()) {
            game->replay->pause();
        } else {
            game->replay->play();
        }
    });
}

void InReplay::render() {
    speedButton->render();
    pauseToggle->render();
    timeline->render();
}

void InReplay::update() {
    speedButton->update();
    pauseToggle->update();
    timeline->update();
}

void InReplay::resize() {

}