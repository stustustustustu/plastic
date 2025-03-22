#include "InReplay.h"

#include "../../../../Game.h"

const auto game = Game::getInstance();

InReplay::InReplay() : Scene("IN_REPLAY") {
    speedButton = std::make_unique<Button>(glm::vec2(border, game -> getSize().y - 2 * (48 + border)), glm::vec2(48, 48), std::to_string(static_cast<int>(game -> replay -> getPlaybackSpeed())) + "x", HEXtoRGB(0x3F3F3F));
    pauseToggle = std::make_unique<Button>(glm::vec2(border, game -> getSize().y - 48 - border), glm::vec2(48, 48), "", HEXtoRGB(0x2F2F2F));
    timeline = std::make_unique<Slider>(glm::vec2(2 * border + 48, game -> getSize().y - 24 - border), glm::vec2(game -> getSize().x - 3 * border - 48, 24), 0, std::chrono::duration_cast<std::chrono::seconds>(game -> replay -> getCurrentReplayDuration()).count(), 0, false);

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
        auto time = std::chrono::milliseconds(static_cast<int>(timeline -> getValue()));
        game -> replay -> seek(time);
    });
}

void InReplay::render() {
    int hex;

    if (pauseToggle -> isHovering()) {
        hex = RGBtoHex(brightness(!game -> replay -> isPlaying() ? 0xFF2222 : 0x22FF22, 0.25));
    } else {
        hex = !game -> replay -> isPlaying() ? 0xFF2222 : 0x22FF22;
    }

    game -> renderer -> DrawSpriteSheet(*game -> texture, pauseToggle -> getPosition() + glm::vec2(2), 2, 32, 32, glm::vec2(48, 48), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawSpriteSheet(*game -> texture, pauseToggle -> getPosition(), 2, 32, 32, glm::vec2(48, 48), 0, HEXtoRGB(hex));

    speedButton -> render();

    // 00:00
    game -> renderer -> DrawSpriteSheet(*game -> texture, timeline -> getPosition() - glm::vec2(-border / 2, border) - glm::vec2(2, 14),
                                        2, 32, 32, glm::vec2(game -> text -> GetWidth("00:00", 16) + 6, 20), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawSpriteSheet(*game -> texture, timeline -> getPosition() - glm::vec2(-border / 2, border) - glm::vec2(4, 16),
                                        2, 32, 32, glm::vec2(game -> text -> GetWidth("00:00", 16) + 6, 20), 0, HEXtoRGB(0x3F3F3F));

    game -> renderer -> DrawText("00:00", timeline -> getPosition() - glm::vec2(-border / 2, border), 16, true);

    // relative end
    auto totalSeconds = std::chrono::duration_cast<std::chrono::seconds>(
        game -> replay -> getCurrentReplayDuration() - game -> replay -> getCurrentTime()
    ).count();

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::string text = "-";
    if (hours > 0) {
        text += std::to_string(hours) + ":" +
               (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
               (seconds < 10 ? "0" : "") + std::to_string(seconds);
    } else {
        text += (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
               (seconds < 10 ? "0" : "") + std::to_string(seconds);
    }

    game -> renderer -> DrawSpriteSheet(*game -> texture, timeline -> getPosition() + glm::vec2(timeline -> getSize().x - game -> text -> GetWidth(text, 16) - 6, 0) - glm::vec2(-border / 2, border) - glm::vec2(2, 14),
                                        2, 32, 32, glm::vec2(game -> text -> GetWidth(text, 16) + 6, 20), 0, HEXtoRGB(0x000000));
    game -> renderer -> DrawSpriteSheet(*game -> texture, timeline -> getPosition() + glm::vec2(timeline -> getSize().x - game -> text -> GetWidth(text, 16) - 6, 0) - glm::vec2(-border / 2, border) - glm::vec2(4, 16),
                                        2, 32, 32, glm::vec2(game -> text -> GetWidth(text, 16) + 6, 20), 0, HEXtoRGB(0x3F3F3F));

    game -> renderer -> DrawText(text,
                                timeline -> getPosition() + glm::vec2(timeline -> getSize().x - game -> text -> GetWidth(text, 16) - 6, 0) - glm::vec2(-border / 2, border),
                                16, true);

    timeline -> render();

    if (game -> getCurrentWorld()) {
        game -> getCurrentWorld() -> render();
    }
}

void InReplay::update() {
    speedButton -> update();
    pauseToggle -> update();
    timeline -> update();

    if (game -> replay -> isPlaying()) {
        auto currentTime = game -> replay -> getCurrentTime();
        auto duration = game -> replay -> getCurrentReplayDuration();
        timeline -> setValue(currentTime.count());

        if (currentTime >= duration) {
            game -> replay -> setPlaying(false);
            timeline -> setValue(duration.count());
        }

        game -> replay -> update(*game -> getCurrentWorld());
    } else {
        auto currentTime = game -> replay -> getCurrentTime();
        timeline -> setValue(currentTime.count());
    }
}

void InReplay::resize() {
    speedButton -> setPosition(glm::vec2(border, game -> getSize().y - 2 * (48 + border)));
    pauseToggle -> setPosition(glm::vec2(border, game -> getSize().y - 48 - border));

    timeline -> setPosition(glm::vec2(2 * border + 48, game -> getSize().y - 24 - border));
    timeline -> setSize(glm::vec2(game -> getSize().x - 3 * border - 48, 24));
    timeline -> setConstraints(0, game -> replay -> getCurrentReplayDuration().count(), false);
}