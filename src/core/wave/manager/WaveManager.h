#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include "../Wave.h"

class WaveManager {
    private:
        std::vector<Wave> waves;

        int currentWaveIndex = 0;
        bool forcable;

        std::deque<std::shared_ptr<Wave>> activeWaves;
        std::chrono::steady_clock::time_point lastWaveSpawnTime;

    public:
        WaveManager();

        void startNextWave();
        void forceNextWave();

        void updateWaveStatus();
        void removeCompletedWaves();

        std::vector<Enemy> *getCurrentEnemies() const;

        void addWave(Wave wave);

        bool isActive() const;
};

#endif //WAVEMANAGER_H
