#include "Replay.h"

void Replay::addEvent(const Event &event) {
    this -> events.push_back(event);
}

std::vector<Event> Replay::getEvents() const {
    return this -> events;
}

std::chrono::milliseconds Replay::getDuration() const {
    return this -> duration;
}

void Replay::setDuration(std::chrono::milliseconds duration) {
    this -> duration = duration;
}

void Replay::setInitialWorldState(const InitialState &state) {
    this -> initial = state;
}

const InitialState& Replay::getInitialWorldState() const {
    return this -> initial;
}

void Replay::setStartTime(std::chrono::steady_clock::time_point startTime) {
    this -> startTime = startTime;
}

std::chrono::steady_clock::time_point Replay::getStartTime() const {
    return this -> startTime;
}

void Replay::save(const std::string& path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save replay: " << path << std::endl;
        return;
    }

    size_t nameLength = initial.name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(initial.name.c_str(), nameLength);

    file.write(reinterpret_cast<const char*>(&initial.seed), sizeof(initial.seed));
    file.write(reinterpret_cast<const char*>(&initial.windowSize), sizeof(initial.windowSize));

    file.write(reinterpret_cast<const char*>(&duration), sizeof(duration));

    size_t eventCount = events.size();
    file.write(reinterpret_cast<const char*>(&eventCount), sizeof(eventCount));

    for (const auto& event : events) {
        file.write(reinterpret_cast<const char*>(&event.timestamp), sizeof(event.timestamp));

        file.write(reinterpret_cast<const char*>(&event.type), sizeof(event.type));

        size_t dataSize = event.data.size();
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

        if (dataSize > 0) {
            file.write(reinterpret_cast<const char*>(event.data.data()), dataSize);
        }
    }

    file.close();
}

void Replay::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to load replay: " << path << std::endl;
        return;
    }

    events.clear();

    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    initial.name.resize(nameLength);
    file.read(&initial.name[0], nameLength);

    file.read(reinterpret_cast<char*>(&initial.seed), sizeof(initial.seed));
    file.read(reinterpret_cast<char*>(&initial.windowSize), sizeof(initial.windowSize));

    file.read(reinterpret_cast<char*>(&duration), sizeof(duration));
    std::cout << duration << std::endl;

    size_t eventCount;
    file.read(reinterpret_cast<char*>(&eventCount), sizeof(eventCount));

    events.reserve(eventCount);
    for (size_t i = 0; i < eventCount; ++i) {
        Event event;

        file.read(reinterpret_cast<char*>(&event.timestamp), sizeof(event.timestamp));

        file.read(reinterpret_cast<char*>(&event.type), sizeof(event.type));

        size_t dataSize;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));

        if (dataSize > 0) {
            event.data.resize(dataSize);
            file.read(reinterpret_cast<char*>(event.data.data()), dataSize);
        }

        events.push_back(event);
    }

    file.close();
}