#ifndef REPLAYMANAGER_H
#define REPLAYMANAGER_H

#include "../src/config.h"
#include "../Replay.h"

class World;

class ReplayManager  {
    private:
        std::unique_ptr<Replay> currentReplay;
        std::chrono::milliseconds currentTime;
        float playbackSpeed;
        bool playing;

    public:
        ReplayManager();

        void loadReplay(const std::string& path);
        void seek(std::chrono::milliseconds time);

        void setPlaybackSpeed(float speed);
        float getPlaybackSpeed() const;

        std::chrono::milliseconds getCurrentTime() const;
        std::chrono::milliseconds getCurrentReplayDuration() const;

        bool isPlaying() const;
        void setPlaying(bool playing);

        void update(World& world);
};

#endif //REPLAYMANAGER_H
