#include "ReplayManager.h"

#include "../src/Game.h"

auto const game = Game::getInstance();

ReplayManager::ReplayManager(): currentTime(0), playbackSpeed(1.0f), playing(false), lastUpdateTime(std::chrono::steady_clock::now()) {}

void ReplayManager::loadReplay(const std::string &path) {
    this -> currentReplay = std::make_unique<Replay>();
    this -> currentReplay -> load(path);

    const auto& initialState = currentReplay -> getInitialWorldState();

    game -> setSize(initialState.windowSize);

    tempWorld = std::make_unique<World>(initialState.name, initialState.seed, MEDIUM);
    tempWorld -> init();
}

void ReplayManager::seek(std::chrono::milliseconds time) {
    this -> currentTime = time;
}

void ReplayManager::setPlaybackSpeed(float speed) {
    this -> playbackSpeed = speed;
}

float ReplayManager::getPlaybackSpeed() const {
    return this -> playbackSpeed;
}

std::chrono::milliseconds ReplayManager::getCurrentTime() const {
    return this -> currentTime;
}

std::chrono::milliseconds ReplayManager::getCurrentReplayDuration() const {
    if (currentReplay) {
        return this -> currentReplay -> getDuration();
    }
    return std::chrono::milliseconds(0);
}

bool ReplayManager::isPlaying() const {
    return this -> playing;
}

void ReplayManager::setPlaying(bool playing) {
    this -> playing = playing;
}

void ReplayManager::update(World& world) {
    if (tempWorld && isPlaying()) {
        auto currentTimePoint = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimePoint - lastUpdateTime);

        auto scaledElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(elapsedTime) * playbackSpeed
        );

        currentTime += scaledElapsedTime;

        lastUpdateTime = currentTimePoint;

        if (currentTime >= currentReplay -> getDuration()) {
            currentTime = currentReplay -> getDuration();
            setPlaying(false);
        }

        currentReplay -> setDuration(currentReplay -> getDuration());
    }
}

void ReplayManager::render(World& world) {
    if (tempWorld) {
        tempWorld -> render();
    }
}