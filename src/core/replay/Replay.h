#ifndef REPLAY_H
#define REPLAY_H

#include <queue>

#include "../../entity/turret/Turret.h"
#include "../src/config.h"

enum class EventType {
    PLAYER_MOVE,
    PLAYER_SHOOT,
    TURRET_PLACE,
    TURRET_UPGRADE,
    ENEMY_SPAWN,
    ENEMY_MOVE,
    ENEMY_DIE,
};

struct InitialState {
    std::string name;
    unsigned int seed;
    glm::vec2 windowSize;
};

struct Event {
    std::chrono::milliseconds timestamp;
    EventType type;
    std::vector<uint8_t> data;
};

class Replay {
    private:
        std::vector<Event> events;
        std::chrono::milliseconds duration;
        InitialState initial;
        std::chrono::steady_clock::time_point startTime;

    public:
        void addEvent(const Event &event);
        const std::vector<Event> &getEvents() const;
        std::chrono::milliseconds getDuration() const;
        void setDuration(std::chrono::milliseconds duration);

        void setInitialWorldState(const InitialState& state);
        const InitialState& getInitialWorldState() const;

        void setStartTime(std::chrono::steady_clock::time_point startTime);
        std::chrono::steady_clock::time_point getStartTime() const;

        void save(const std::string &path);
        void load(const std::string &path);
};

#endif //REPLAY_H
