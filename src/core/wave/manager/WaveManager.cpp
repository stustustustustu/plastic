#include "WaveManager.h"

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

Wave* WaveManager::getCurrentWave() {
    return std::move(currentWave);
}

int WaveManager::getWaveIndex(const Wave &wave) const {
    for (const auto& pair : waves) {
        if (&pair.second == &wave) {
            return pair.first;
        }
    }
    return -1;  // Wave not found
}

void WaveManager::startNextWave(GLFWwindow* window) {
    ++currentWaveIndex;
    currentWave = new Wave(5, window);

    std::cout << "Started Wave " << currentWaveIndex << " with " << currentWave -> getEnemies().size() << " enemies." << std::endl;
}
