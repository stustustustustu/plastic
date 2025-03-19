#include "Replay.h"

void Replay::addEvent(const Event &event) {
    this -> events.push_back(event);
}

const std::vector<Event> & Replay::getEvents() const {
    return this -> events;
}

std::chrono::milliseconds Replay::getDuration() const {
    return this -> duration;
}

void Replay::setDuration(std::chrono::milliseconds duration) {
    this -> duration = duration;
}

void Replay::save(const std::string &path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save replay: " << path << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&duration), sizeof(duration));

    size_t eventCount = events.size();
    file.write(reinterpret_cast<const char*>(&eventCount), sizeof(eventCount));

    for (const auto& event : events) {
        file.write(reinterpret_cast<const char*>(&event.timestamp), sizeof(event.timestamp));
        file.write(reinterpret_cast<const char*>(&event.type), sizeof(event.type));

        size_t dataSize = event.data.size();
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
        file.write(reinterpret_cast<const char*>(event.data.data()), dataSize);
    }

    file.close();
}

void Replay::load(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to load replay: " << path << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&duration), sizeof(duration));

    size_t eventCount;
    file.read(reinterpret_cast<char*>(&eventCount), sizeof(eventCount));

    events.clear();
    events.reserve(eventCount);

    for (size_t i = 0; i < eventCount; ++i) {
        Event event;
        file.read(reinterpret_cast<char*>(&event.timestamp), sizeof(event.timestamp));
        file.read(reinterpret_cast<char*>(&event.type), sizeof(event.type));

        size_t dataSize;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        event.data.resize(dataSize);
        file.read(reinterpret_cast<char*>(event.data.data()), dataSize);

        events.push_back(event);
    }

    file.close();
}
