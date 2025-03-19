#ifndef REPLAY_H
#define REPLAY_H

#include "../src/config.h"

enum class EventType {
    PLAYER_MOVE,
    PLAYER_SHOOT,
    TURRET_PLACE,
    TURRET_UPGRADE,
    ENEMY_SPAWN,
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

    public:
        void addEvent(const Event &event);
        const std::vector<Event> &getEvents() const;
        std::chrono::milliseconds getDuration() const;
        void setDuration(std::chrono::milliseconds duration);

        void save(const std::string &path);
        void load(const std::string &path);
};

#endif //REPLAY_H
