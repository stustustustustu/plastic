#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include "../Wave.h"
#include "../src/utils/math/random.cpp"

class WaveManager {
    private:
        std::map<int, Wave> waves;
        int currentWaveIndex = 0;
    public:
        void generateWaves(int n, GLFWwindow *window); // wave number

        void addWave(int index, const Wave &wave);
        bool removeWave(int index);

        bool hasNextWave();

         Wave *getCurrentWave();
        int getWaveIndex(const Wave &wave) const;

        void startNextWave();
};

#endif //WAVEMANAGER_H
