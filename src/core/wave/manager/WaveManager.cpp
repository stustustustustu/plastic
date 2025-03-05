#include "WaveManager.h"
#include "../../../Game.h"

auto const game = Game::getInstance();

WaveManager::WaveManager() : currentWaveIndex(0), forcable(false) {}

void WaveManager::startNextWave() {
    currentWaveIndex++;
    int weight = 100 * std::pow(1.2f, currentWaveIndex);
    bool bossfight = false;

    switch (game -> getDifficulty()) {
        case EASY:
            weight = static_cast<int>(weight * 0.8f);
            break;
        case MEDIUM:
            weight = static_cast<int>(weight * 1.0f);
            break;
        case HARD:
            weight = static_cast<int>(weight * 1.2f);
            break;
        case EXPERT:
            weight = static_cast<int>(weight * 1.4f);
            break;
        case IMPOSSIBLE:
            weight = static_cast<int>(weight * 1.6f);
            break;
    }

    if (currentWaveIndex > 0 && rand() % (100 - (currentWaveIndex * 2)) == 0) {
        weight *= 2;
        bossfight = true;
        std::cout << "Boss wave detected!" << std::endl;
    }

    Wave wave(currentWaveIndex, weight, bossfight);
    std::vector<Enemy> generated = Enemy::generateEnemies(wave.getIndex(), wave.getWeight());
    wave.addEnemies(generated);

    activeWaves.push_back(std::make_shared<Wave>(wave));
    lastWaveSpawnTime = std::chrono::steady_clock::now();
    std::cout << "Starting wave " << wave.getIndex() << " with " << wave.getEnemies().size() << " enemies." << std::endl;
}

void WaveManager::forceNextWave() {
    if (!forcable) return;

    Wave &currentWave = *activeWaves.front();
    int enemyCount = currentWave.getEnemies().size();
    int killedEnemies = std::count_if(currentWave.getEnemies().begin(), currentWave.getEnemies().end(), [](const Enemy& enemy) { return enemy.getHealth() <= 0; });

    if (static_cast<float>(killedEnemies) / enemyCount >= 0.2f) {
        startNextWave();
        forcable = false;
    } else {
        std::cerr << "Can't force next wave! Kill more enemies." << std::endl;
    }
}

void WaveManager::updateWaveStatus() {
    if (activeWaves.empty()) return;

    Wave &currentWave = *activeWaves.front();
    if (std::all_of(currentWave.getEnemies().begin(), currentWave.getEnemies().end(),[](const Enemy& e) { return e.getHealth() <= 0; })) {
        activeWaves.pop_front();
        if (activeWaves.empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            startNextWave();
        }
    }

    auto currentTime = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastWaveSpawnTime).count() >= 5) {
        forcable = true;
    }
}

void WaveManager::removeCompletedWaves() {
    waves.erase(std::remove_if(waves.begin(), waves.end(), [](Wave& wave) {
        return std::all_of(wave.getEnemies().begin(), wave.getEnemies().end(), [](const Enemy& e) { return e.getHealth() <= 0; });
    }), waves.end());
}

std::vector<Enemy> *WaveManager::getCurrentEnemies() const {
    if (activeWaves.empty()) {
        return NULL;
    }

    return &activeWaves.front() -> getEnemies();
}

void WaveManager::addWave(Wave wave) {
    waves.push_back(wave);
}

bool WaveManager::isActive() const {
    return !activeWaves.empty();
}
