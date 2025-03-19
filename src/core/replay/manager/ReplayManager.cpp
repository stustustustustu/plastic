#include "ReplayManager.h"

#include "../src/Game.h"

ReplayManager::ReplayManager(): currentTime(0), playbackSpeed(1.0f), playing(false) {}

void ReplayManager::loadReplay(const std::string &path) {
    this -> currentReplay = std::make_unique<Replay>();
    this -> currentReplay -> load(path);
}

void ReplayManager::play() {
    this -> playing = true;
}

void ReplayManager::pause() {
    this -> playing = false;
}

void ReplayManager::seek(std::chrono::milliseconds time) {
    this -> currentTime = time;
}

void ReplayManager::setPlaybackSpeed(float speed) {
    this -> playbackSpeed = speed;
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

void ReplayManager::update(World &world) {
    if (!playing || !currentReplay) return;

    auto deltaTime = std::chrono::milliseconds(static_cast<int>(16 * playbackSpeed));
    currentTime += deltaTime;

    const auto& events = currentReplay->getEvents();
    for (const auto& event : events) {
        if (event.timestamp <= currentTime) {
            switch (event.type) {
                case EventType::PLAYER_MOVE: {
                    glm::vec2 position = *reinterpret_cast<const glm::vec2*>(event.data.data());
                    world.player -> setPosition({position.x, position.y});
                    break;
                }
                case EventType::PLAYER_SHOOT: {
                    glm::vec2 cursorPos = *reinterpret_cast<const glm::vec2*>(event.data.data());
                    world.player -> shoot();
                    break;
                }
                case EventType::TURRET_PLACE: {
                    TurretType type = *reinterpret_cast<const TurretType*>(event.data.data());
                    glm::vec2 position = *reinterpret_cast<const glm::vec2*>(event.data.data() + sizeof(TurretType));
                    world.turret -> addTurret(type, {position.x, position.y});
                    break;
                }
                case EventType::ENEMY_SPAWN: {
                    EnemyType type = *reinterpret_cast<const EnemyType*>(event.data.data());
                    glm::vec2 position = *reinterpret_cast<const glm::vec2*>(event.data.data() + sizeof(EnemyType));
                    float health = *reinterpret_cast<const float*>(event.data.data() + sizeof(EnemyType) + sizeof(glm::vec2));
                    float damage = *reinterpret_cast<const float*>(event.data.data() + sizeof(EnemyType) + sizeof(glm::vec2) + sizeof(float));
                    float speed = *reinterpret_cast<const float*>(event.data.data() + sizeof(EnemyType) + sizeof(glm::vec2) + sizeof(float) + sizeof(float));
                    world.enemies -> emplace_back(type, std::vector<float>{position.x, position.y}, health, damage, speed);
                    break;
                }
            }
        }
    }
}