#include "WaveManager.h"

void WaveManager::generateWaves(int n, GLFWwindow *window) {
    std::cout << "Generated " << n << " waves." << std::endl;

    for (int i = 0; i < n; i++) {
        Wave wave(5, window);
        addWave(i, wave);
    }

    currentWaveIndex = 0;
}

void WaveManager::addWave(int index, const Wave &wave) {
    waves[index] = wave;
}

bool WaveManager::removeWave(int index) {
    if (waves.erase(index) > 0) {
        if (index <= currentWaveIndex) {
            currentWaveIndex = std::max(0, currentWaveIndex - 1);
        }
        return true;
    }
    return false;
}

bool WaveManager::hasNextWave() {
    return waves.find(currentWaveIndex + 1) != waves.end();
}

Wave* WaveManager::getCurrentWave() {
    const auto it = waves.find(currentWaveIndex);
    if (it == waves.end()) {
        return NULL;
    }
    return &it -> second;
}

int WaveManager::getWaveIndex(const Wave &wave) const {
    for (const auto& pair : waves) {
        if (&pair.second == &wave) {
            return pair.first;
        }
    }
    return -1;  // Wave not found
}

void WaveManager::startNextWave() {
    if (hasNextWave()) {
        ++currentWaveIndex;
    }
}
