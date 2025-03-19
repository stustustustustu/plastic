#include "InReplay.h"

#include "../../../../Game.h"

const auto game = Game::getInstance();

InReplay::InReplay() : Scene("IN_REPLAY") {
    speedButton = std::make_unique<Button>(glm::vec2(border, game -> getSize().at(1) - 2 * (48 + border)), glm::vec2(48, 48), std::to_string(static_cast<int>(game -> replay -> getPlaybackSpeed())) + "x", HEXtoRGB(0x2F2F2F));
    pauseToggle = std::make_unique<Button>(glm::vec2(border, game -> getSize().at(1) - 48 - border), glm::vec2(48, 48), "", HEXtoRGB(0x2F2F2F));
    timeline = std::make_unique<Slider>(glm::vec2(2 * border + 48, game -> getSize().at(1) - 24 - border), glm::vec2(game -> getSize().at(0) - 3 * border - 48, 24), 0, 100, 0, false);

    speedButton -> addCallback([this]() {
        static float speeds[] = {0.5f, 1.0f, 2.0f, 4.0f};
        static int currentSpeed = 1;

        currentSpeed = (currentSpeed + 1) % 4;

        game -> replay -> setPlaybackSpeed(speeds[currentSpeed]);

        std::ostringstream stream;
        currentSpeed == 0 ? stream << std::fixed << std::setprecision(1) << game -> replay -> getPlaybackSpeed() << "x" : stream << game -> replay -> getPlaybackSpeed() << "x";

        speedButton -> setLabel(stream.str());
    });
    
    pauseToggle -> addCallback([this]() {
        game -> replay -> setPlaying(!game -> replay -> isPlaying());
    });

    timeline -> addCallback([this]() {
        auto duration = game -> replay -> getCurrentReplayDuration();
        auto time = std::chrono::milliseconds(static_cast<int>(timeline -> getValue() * duration.count()));
        game -> replay -> seek(time);
    });
}

void InReplay::render() {
    int hex;

    if (pauseToggle -> isHovering()) {
        hex = RGBtoHex(brightness(game -> replay -> isPlaying() ? 0xFF2222 : 0x22FF22, 0.25));
    } else {
        hex = game -> replay -> isPlaying() ? 0xFF2222 : 0x22FF22;
    }

    game -> renderer -> DrawSpriteSheet(*game -> texture, pauseToggle -> getPosition() + glm::vec2(2), 2, 32, 32, glm::vec2(48, 48), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawSpriteSheet(*game -> texture, pauseToggle -> getPosition(), 2, 32, 32, glm::vec2(48, 48), 0, HEXtoRGB(hex));

    speedButton -> render();
    //pauseToggle -> render();
    timeline -> render();
}

void InReplay::update() {
    speedButton -> update();
    pauseToggle -> update();
    timeline -> update();
}

void InReplay::resize() {
    speedButton -> setPosition(glm::vec2(border, game -> getSize().at(1) - 2 * (48 + border)));
    pauseToggle -> setPosition(glm::vec2(border, game -> getSize().at(1) - 48 - border));
    timeline -> setPosition(glm::vec2(2 * border + 48, game -> getSize().at(1) - 24 - border));
    timeline -> setSize(glm::vec2(game -> getSize().at(0) - 3 * border - 48, 24));
}