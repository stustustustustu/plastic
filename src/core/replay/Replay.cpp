#include "Replay.h"

void Replay::addEvent(const Event &event) {
    this -> events.push_back(event);
}

const std::vector<Event>& Replay::getEvents() const {
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

    file.close();
}

void Replay::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to load replay: " << path << std::endl;
        return;
    }

    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    initial.name.resize(nameLength);
    file.read(&initial.name[0], nameLength);

    file.read(reinterpret_cast<char*>(&initial.seed), sizeof(initial.seed));
    file.read(reinterpret_cast<char*>(&initial.windowSize), sizeof(initial.windowSize));

    file.read(reinterpret_cast<char*>(&duration), sizeof(duration));

    file.close();
}