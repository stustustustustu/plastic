#include "World.h"
#include "../../Game.h"
#include "../UI/scene/scenes/InGame.h"

const auto game = Game::getInstance();

World::World(std::string name, unsigned int seed, Difficulty difficulty)
    : name(name), seed(seed), difficulty(difficulty), score(0), enemies(new std::vector<Enemy>()) {}

void World::init() {
    island = std::make_unique<Island>(seed);
    wave = std::make_unique<WaveManager>();
    inventory = std::make_unique<Inventory>();
    upgrade = std::make_unique<UpgradeManager>(*inventory);
    turret = std::make_unique<TurretManager>();
    player = std::make_unique<Player>();

    replay = std::make_unique<Replay>();
    replay -> setStartTime(std::chrono::steady_clock::now());

    wave -> startNextWave();
}

void World::initial() {
    std::filesystem::path folder = std::filesystem::path("saves") / name;
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directories(folder);
    }

    InitialState state;
    state.name = name;
    state.seed = seed;
    state.windowSize = game -> getSize();

    replay -> setDuration(std::chrono::milliseconds(0));
    replay -> setInitialWorldState(state);

    std::filesystem::path replayPath = folder / "replay.bin";
    replay -> save(replayPath.string());
}

void World::save(const std::string &world) {
    std::filesystem::path folder = std::filesystem::path("saves") / world;
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directories(folder);
    }

    std::filesystem::path path = folder / "save.bin";
    std::ofstream file(folder / "save.bin", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save world: " << path << std::endl;
        return;
    }

    // name
    size_t nameLength = name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(name.c_str(), nameLength);

    // seed, difficulty, score and wave index
    file.write(reinterpret_cast<const char*>(&seed), sizeof(seed));
    file.write(reinterpret_cast<const char*>(&difficulty), sizeof(difficulty));
    file.write(reinterpret_cast<const char*>(&score), sizeof(score));
    int currentWaveIndex = wave -> getCurrentWaveIndex();
    file.write(reinterpret_cast<const char*>(&currentWaveIndex), sizeof(currentWaveIndex));

    // window size (glm::vec2)
    glm::vec2 windowSize = game -> getSize();
    file.write(reinterpret_cast<const char*>(&windowSize), sizeof(windowSize));

    // player data
    float playerHealth = player -> getHealth();
    float playerMaxHealth = player -> getMaxHealth();

    float playerShield = player -> getShield();
    float playerMaxShield = player -> getMaxShield();

    float playerDamage = player -> getDamage();
    float playerSpeed = player -> getSpeed();

    int playerCoins = player -> getCoins();

    glm::vec2 playerPosition = player -> getPosition();

    file.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    file.write(reinterpret_cast<const char*>(&playerMaxHealth), sizeof(playerMaxHealth));

    file.write(reinterpret_cast<const char*>(&playerShield), sizeof(playerShield));
    file.write(reinterpret_cast<const char*>(&playerMaxShield), sizeof(playerMaxShield));

    file.write(reinterpret_cast<const char*>(&playerDamage), sizeof(playerDamage));
    file.write(reinterpret_cast<const char*>(&playerSpeed), sizeof(playerSpeed));

    file.write(reinterpret_cast<const char*>(&playerCoins), sizeof(playerCoins));

    file.write(reinterpret_cast<const char*>(&playerPosition), sizeof(playerPosition));

    // player upgrades
    size_t upgradeCount = upgrade -> getPaths().size();
    file.write(reinterpret_cast<const char*>(&upgradeCount), sizeof(upgradeCount));

    for (const auto& [type, path] : upgrade -> getPaths()) {
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        int level = path.getLevel();
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
    }

    // turret data
    size_t turretCount = turret -> getTurrets().size();
    file.write(reinterpret_cast<const char*>(&turretCount), sizeof(turretCount));

    for (const auto& turretInstance : turret -> getTurrets()) {
        TurretType type = turretInstance -> getType();
        glm::vec2 position = turretInstance -> getPosition();

        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&position), sizeof(position));
    }

    // wave and enemies data
    size_t enemyCount = enemies -> size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));

    for (const auto& enemy : *enemies) {
        EnemyType type = enemy.getType();
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));

        glm::vec2 position = enemy.getPosition();
        file.write(reinterpret_cast<const char*>(&position), sizeof(position));

        float health = enemy.getHealth();
        float damage = enemy.getDamage();
        float speed = enemy.getSpeed();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&speed), sizeof(speed));
    }

    file.close();

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - replay -> getStartTime());
    replay -> setDuration(elapsedTime);

    std::filesystem::path replayPath = folder / "replay.bin";
    replay -> save(replayPath.string());
}

void World::load(const std::string &world) {
    std::filesystem::path folder = std::filesystem::path("saves") / world;
    std::filesystem::path savePath = folder / "save.bin";
    if (!std::filesystem::exists(savePath)) {
        std::cerr << "Save file does not exist: " << savePath << std::endl;
        return;
    }

    std::ifstream file(savePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to load world: " << savePath << std::endl;
        return;
    }

    // name
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    std::string loadedName(nameLength, '\0');
    file.read(&loadedName[0], nameLength);
    name = loadedName;

    // seed, difficulty, and wave index
    file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    int currentWaveIndex;
    file.read(reinterpret_cast<char*>(&currentWaveIndex), sizeof(currentWaveIndex));

    // window size
    glm::vec2 windowSize;
    file.read(reinterpret_cast<char*>(&windowSize), sizeof(windowSize));
    game -> setSize(windowSize);

    init();

    wave -> setCurrentWaveIndex(currentWaveIndex);

    // player data
    float playerHealth;
    float playerMaxHealth;

    float playerShield;
    float playerMaxShield;

    float playerDamage;
    float playerSpeed;

    int playerCoins;

    glm::vec2 playerPosition;

    file.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    file.read(reinterpret_cast<char*>(&playerMaxHealth), sizeof(playerMaxHealth));

    file.read(reinterpret_cast<char*>(&playerShield), sizeof(playerShield));
    file.read(reinterpret_cast<char*>(&playerMaxShield), sizeof(playerMaxShield));

    file.read(reinterpret_cast<char*>(&playerDamage), sizeof(playerDamage));
    file.read(reinterpret_cast<char*>(&playerSpeed), sizeof(playerSpeed));

    file.read(reinterpret_cast<char*>(&playerCoins), sizeof(playerCoins));

    file.read(reinterpret_cast<char*>(&playerPosition), sizeof(playerPosition));

    player -> setHealth(playerHealth);
    player -> setMaxHealth(playerMaxHealth);

    player -> setShield(playerShield);
    player -> setMaxShield(playerMaxShield);

    player -> setDamage(playerDamage);
    player -> setSpeed(playerSpeed);

    player -> setCoins(playerCoins);

    player -> setPosition(playerPosition);

    // player upgrades
    size_t upgradeCount;
    file.read(reinterpret_cast<char*>(&upgradeCount), sizeof(upgradeCount));

    std::map<UpgradeType, int> levels;

    for (size_t i = 0; i < upgradeCount; ++i) {
        UpgradeType type;
        int level;

        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&level), sizeof(level));

        levels[type] = level;
    }

    upgrade -> loadPaths(levels);

    auto scene = game -> scenes -> getScene("IN_GAME");
    if (scene) {
        InGame* inGameScene = dynamic_cast<InGame*>(scene);
        if (inGameScene) {
            inGameScene -> refreshUpgradePanels();
        }
    }

    // turret data
    size_t turretCount;
    file.read(reinterpret_cast<char*>(&turretCount), sizeof(turretCount));

    for (size_t i = 0; i < turretCount; ++i) {
        TurretType type;
        glm::vec2 position;

        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&position), sizeof(position));

        turret -> addTurret(type, position);
    }

    // waves and enemies data
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));

    enemies -> clear();

    for (size_t i = 0; i < enemyCount; ++i) {
        EnemyType type;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));

        glm::vec2 position;
        file.read(reinterpret_cast<char*>(&position), sizeof(position));

        float health, damage, speed;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        file.read(reinterpret_cast<char*>(&speed), sizeof(speed));

        Enemy enemy(type, position, health, damage, speed * 2);
        enemies -> push_back(enemy);
    }

    wave -> setCurrentEnemies(*enemies);

    file.close();

    std::filesystem::path replayPath = folder / "replay.bin";
    replay -> load(replayPath.string());
    replay -> setStartTime(std::chrono::steady_clock::now());
}

void World::update() {
    enemies = wave -> getCurrentEnemies();

    for (auto &projectile : projectiles) {
        projectile -> update();
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
        [](const std::unique_ptr<Projectile> &projectile) {
                return projectile -> isMarked();
            }), projectiles.end()
    );

    for (auto &explosion : explosions) {
        explosion -> update();
    }

    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(),
        [](const std::unique_ptr<Explosion> &explosion) {
            return explosion -> isFinished();
        }), explosions.end()
    );

    for (int i = 0; i < enemies -> size();) {
        (*enemies)[i].moveTowards(player -> getPosition());

        if (island -> isLand(static_cast<int>((*enemies)[i].getPosition().x / Island::TILE_SIZE),
                            static_cast<int>((*enemies)[i].getPosition().y / Island::TILE_SIZE))) {
            player -> hit((*enemies)[i].getDamage(), false);

            Event event;

            event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - replay -> getStartTime()
            );
            event.type = EventType::ENEMY_DIE;

            auto type = (*enemies)[i].getType();
            event.data.resize(sizeof(EnemyType));
            memcpy(event.data.data(), &type, sizeof(EnemyType));

            replay -> addEvent(event);

            enemies -> erase(enemies -> begin() + i);
        }

        if ((*enemies)[i].getHealth() <= 0) {
            this -> setScore(getScore() + (*enemies)[i].getCoins());
            player -> takeCoins((*enemies)[i], 1.0f);

            Event event;

            event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - replay -> getStartTime()
            );
            event.type = EventType::ENEMY_DIE;

            auto type = (*enemies)[i].getType();
            event.data.resize(sizeof(EnemyType));
            memcpy(event.data.data(), &type, sizeof(EnemyType));

            replay -> addEvent(event);

            enemies -> erase(enemies -> begin() + i);
        } else {
            ++i;
        }
    }

    if (enemies -> empty()) {
        wave -> startNextWave();
        wave -> updateWaveStatus();
        enemies = wave -> getCurrentEnemies();
    }

    player -> Movement();

    turret -> update();
}

void World::render() {
    game -> renderer -> SetProjection(game -> camera -> getCameraProjection());

    island -> render(*game -> texture);

    for (auto &projectile : projectiles) {
        projectile -> render();
    }

    for (auto &explosion : explosions) {
        explosion -> render();
    }

    if (wave) {
        for (const auto& enemy : *enemies) {
            if (enemy.getHealth() > 0) {
                enemy.drawEntity(game -> texture);
            }
        }
    }

    if (player -> getHealth() > 0) {
        player -> drawEntity(game -> texture);
    }

    player -> drawLaser();

    turret -> render();
}