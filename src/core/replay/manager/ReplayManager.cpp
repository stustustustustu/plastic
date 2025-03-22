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

void ReplayManager::applyEvent(World& world, const Event& event) {
    if (!world.player) {
        std::cerr << "Error: World or player is not initialized!" << std::endl;
        return;
    }

    switch (event.type) {
        case EventType::PLAYER_MOVE: {
            if (event.data.size() < sizeof(glm::vec2)) {
                std::cerr << "Error: Invalid event data size for PLAYER_MOVE!" << std::endl;
                return;
            }

            glm::vec2 position = *reinterpret_cast<const glm::vec2*>(event.data.data());
            world.player -> setPosition(position);

            break;
        }

        case EventType::PLAYER_SHOOT: {
            if (event.data.size() < sizeof(glm::vec2)) {
                std::cerr << "Error: Invalid event data size for PLAYER_SHOOT!" << std::endl;
                return;
            }

            glm::vec2 targetPos = *reinterpret_cast<const glm::vec2*>(event.data.data());
            world.player -> shoot(targetPos);

            break;
        }

        case EventType::TURRET_PLACE: {
            if (event.data.size() < sizeof(TurretType) + sizeof(glm::vec2)) {
                std::cerr << "Error: Invalid event data size for TURRET_PLACE!" << std::endl;
                return;
            }

            TurretType type = *reinterpret_cast<const TurretType*>(event.data.data());
            glm::vec2 position = *reinterpret_cast<const glm::vec2*>(event.data.data() + sizeof(TurretType));

            world.turret -> addTurret(type, position);

            break;
        }

        default:
            std::cerr << "Unknown event type: " << static_cast<int>(event.type) << std::endl;
            break;
    }
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

        const auto& events = currentReplay -> getEvents();
        for (const auto& event : events) {
            if (event.timestamp <= currentTime) {
                applyEvent(*tempWorld, event);
            }
        }
    }
}

void ReplayManager::render(World& world) {
    if (tempWorld) {
        tempWorld -> render();
    }
}