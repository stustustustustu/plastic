#include "ReplayManager.h"

#include "../src/Game.h"

auto const game = Game::getInstance();

ReplayManager::ReplayManager(): currentTime(0), playbackSpeed(1.0f), playing(false), lastUpdateTime(std::chrono::steady_clock::now()) {}

void ReplayManager::loadReplay(const std::string& path) {
    this -> currentReplay = std::make_unique<Replay>();
    this -> currentReplay -> load(path);

    events = currentReplay -> getEvents();
    currentEventIndex = 0;

    const auto& initialState = currentReplay -> getInitialWorldState();
    game -> setSize(initialState.windowSize);

    tempWorld = std::make_unique<World>(initialState.name, initialState.seed, MEDIUM);
    tempWorld -> init();
}

void ReplayManager::seek(std::chrono::milliseconds time) {
    if (!currentReplay) return;

    tempWorld -> turret -> clearTurrets();
    tempWorld -> enemies -> clear();

    currentEventIndex = 0;

    this -> currentTime = time;
    while (currentEventIndex < events.size() && events[currentEventIndex].timestamp <= currentTime) {
        applyEvent(*tempWorld, events[currentEventIndex]);
        currentEventIndex++;
    }
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

        case EventType::ENEMY_SPAWN: {
            if (event.data.size() < sizeof(EnemyType) + sizeof(glm::vec2)) {
                std::cerr << "Error: Invalid event data size for ENEMY_SPAWN!" << std::endl;
                return;
            }

            EnemyType type = *reinterpret_cast<const EnemyType*>(event.data.data());
            glm::vec2 position = *reinterpret_cast<const glm::vec2*>(event.data.data() + sizeof(EnemyType));
            float speed = *reinterpret_cast<const float*>(event.data.data() + sizeof(EnemyType) + 2 * sizeof(glm::vec2));

            Enemy enemy(type, position, 100, 50, speed);
            enemy.setSpawn(position, event.timestamp);
            world.enemies -> push_back(enemy);

            break;
        }

        case EventType::ENEMY_MOVE: {
            if (event.data.size() < sizeof(EnemyType) + sizeof(glm::vec2)) {
                std::cerr << "Error: Invalid event data size for ENEMY_MOVE!" << std::endl;
                return;
            }

            EnemyType type = *reinterpret_cast<const EnemyType*>(event.data.data());
            glm::vec2 position = *reinterpret_cast<const glm::vec2*>(event.data.data() + sizeof(EnemyType));

            for (auto& enemy : *world.enemies) {
                if (enemy.getType() == type) {
                    enemy.setPosition(position);
                    break;
                }
            }

            break;
        }

        case EventType::ENEMY_DIE: {
            if (event.data.size() < sizeof(EnemyType)) {
                std::cerr << "Error: Invalid event data size for ENEMY_DIE!" << std::endl;
                return;
            }

            EnemyType type = *reinterpret_cast<const EnemyType*>(event.data.data());

            world.enemies -> erase(
                std::remove_if(
                    world.enemies -> begin(),
                    world.enemies -> end(),
                    [type](const Enemy& enemy) { return enemy.getType() == type; }
                ),
                world.enemies -> end()
            );

            break;
        }

        default:
            std::cerr << "Unknown event type: " << static_cast<int>(event.type) << std::endl;
            break;
    }
}

void ReplayManager::update() {
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
            return;
        }

        while (currentEventIndex < events.size() && events[currentEventIndex].timestamp <= currentTime) {
            applyEvent(*tempWorld, events[currentEventIndex]);
            currentEventIndex++;
        }
    }
}

void ReplayManager::render() {
    if (tempWorld) {
        tempWorld -> render();

        game -> renderer -> SetProjection(game -> camera -> getCameraProjection());
        if (debug) {
            for (auto& enemy : *tempWorld -> enemies) {
                game -> renderer -> DrawLine(
                    enemy.getRenderPosition() + glm::ivec2(16),
                    enemy.getRenderPosition() + glm::ivec2(16) + glm::ivec2(normalize(glm::vec2(tempWorld -> player -> getRenderPosition() - enemy.getRenderPosition())) * 100.0f * enemy.getSpeed()),
                    1.0f, HEXtoRGB(0x000000)
                );
            }
        }
    }
}