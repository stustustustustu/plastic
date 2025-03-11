#include "World.h"
#include "../../Game.h"

const auto game = Game::getInstance();

World::World(std::string name, unsigned int seed, Difficulty difficulty) : name(name), seed(seed), difficulty(difficulty) {}

void World::init() {
    island = std::make_unique<Island>(seed);
    wave = std::make_unique<WaveManager>();
    inventory = std::make_unique<Inventory>();
    upgrade = std::make_unique<UpgradeManager>(*inventory);
    turret = std::make_unique<TurretManager>();
    player = std::make_unique<Player>();

    wave -> startNextWave();
}

void World::save(const std::string &path) {
    std::string fileName = path + "/" + name + ".bin";
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save world: " << path << std::endl;
        return;
    }

    // Save name, seed and difficulty
    file.write(reinterpret_cast<const char*>(&name), sizeof(name));
    file.write(reinterpret_cast<const char*>(&seed), sizeof(seed));
    file.write(reinterpret_cast<const char*>(&difficulty), sizeof(difficulty));

    // Save window size
    auto windowSize = game->getSize();
    file.write(reinterpret_cast<const char*>(&windowSize[0]), sizeof(windowSize[0]));
    file.write(reinterpret_cast<const char*>(&windowSize[1]), sizeof(windowSize[1]));

    // Save player data
    float playerHealth = player->getHealth();
    int playerCoins = player->getCoins();
    glm::vec2 playerPosition = glm::vec2(player->getPosition().at(0), player->getPosition().at(1));

    file.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    file.write(reinterpret_cast<const char*>(&playerCoins), sizeof(playerCoins));
    file.write(reinterpret_cast<const char*>(&playerPosition), sizeof(playerPosition));

    // Save upgrade manager data
    size_t upgradePathCount = upgrade->getPaths().size();
    file.write(reinterpret_cast<const char*>(&upgradePathCount), sizeof(upgradePathCount));

    for (const auto& [type, path] : upgrade->getPaths()) {
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        int level = path.getLevel();
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
    }

    // Save turret upgrade manager data
    for (TurretType type : {TurretType::LASER, TurretType::RIFLE, TurretType::BOMB}) {
        const auto& turretUpgrades = turret->getUpgradeManager().getSkillTree(type);
        size_t upgradeCount = turretUpgrades.size();
        file.write(reinterpret_cast<const char*>(&upgradeCount), sizeof(upgradeCount));

        for (const auto& [upgrade, prerequisites] : turretUpgrades) {
            std::string name = upgrade->getName();
            size_t nameLength = name.size();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
            file.write(name.c_str(), nameLength);

            bool isUnlocked = upgrade->isUnlocked();
            file.write(reinterpret_cast<const char*>(&isUnlocked), sizeof(isUnlocked));
        }
    }

    // Save wave manager data
    int currentWaveIndex = wave->getCurrentWaveIndex();
    file.write(reinterpret_cast<const char*>(&currentWaveIndex), sizeof(currentWaveIndex));

    size_t activeWaveCount = wave->getCurrentEnemies()->size();
    file.write(reinterpret_cast<const char*>(&activeWaveCount), sizeof(activeWaveCount));

    for (const auto& enemy : *wave->getCurrentEnemies()) {
        float enemyHealth = enemy.getHealth();
        glm::vec2 enemyPosition = glm::vec2(enemy.getPosition().at(0), enemy.getPosition().at(1));
        EnemyType enemyType = enemy.getType();

        file.write(reinterpret_cast<const char*>(&enemyHealth), sizeof(enemyHealth));
        file.write(reinterpret_cast<const char*>(&enemyPosition), sizeof(enemyPosition));
        file.write(reinterpret_cast<const char*>(&enemyType), sizeof(enemyType));
    }

    file.close();
}

void World::load(const std::string &path) {
    std::string fileName = path + "/" + name + ".bin";
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to load world: " << path << std::endl;
        return;
    }

    // Load name, seed and difficulty
    file.read(reinterpret_cast<char*>(&name), sizeof(name));
    file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));

    // Load window size
    int windowWidth, windowHeight;
    file.read(reinterpret_cast<char*>(&windowWidth), sizeof(windowWidth));
    file.read(reinterpret_cast<char*>(&windowHeight), sizeof(windowHeight));

    // Set the window size in the Game instance
    game->setSize(std::to_string(windowWidth) + "x" + std::to_string(windowHeight));

    // Reinitialize the world
    init();

    // Load player data
    float playerHealth;
    int playerCoins;
    glm::vec2 playerPosition;

    file.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    file.read(reinterpret_cast<char*>(&playerCoins), sizeof(playerCoins));
    file.read(reinterpret_cast<char*>(&playerPosition), sizeof(playerPosition));

    player->setHealth(playerHealth);
    player->setCoins(playerCoins);
    player->setPosition({playerPosition.x, playerPosition.y});

    // Load inventory data
    int coins;
    file.read(reinterpret_cast<char*>(&coins), sizeof(coins));
    inventory->addCoins(coins);

    // Load upgrade manager data
    size_t upgradePathCount;
    file.read(reinterpret_cast<char*>(&upgradePathCount), sizeof(upgradePathCount));

    for (size_t i = 0; i < upgradePathCount; ++i) {
        UpgradeType type;
        int level;

        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&level), sizeof(level));

        if (upgrade->getPaths().find(type) != upgrade->getPaths().end()) {
            for (int j = 0; j < level; ++j) {
                upgrade->upgrade(type); // Apply upgrades to reach the saved level
            }
        }
    }

    // Load turret upgrade manager data
    size_t turretTypeCount;
    file.read(reinterpret_cast<char*>(&turretTypeCount), sizeof(turretTypeCount));

    for (size_t i = 0; i < turretTypeCount; ++i) {
        TurretType type;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));

        size_t upgradeCount;
        file.read(reinterpret_cast<char*>(&upgradeCount), sizeof(upgradeCount));

        for (size_t j = 0; j < upgradeCount; ++j) {
            size_t nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);

            bool isUnlocked;
            file.read(reinterpret_cast<char*>(&isUnlocked), sizeof(isUnlocked));

            for (auto& upgrade : turret->getUpgradeManager().getAvailableUpgrades(type)) {
                if (upgrade->getName() == name) {
                    upgrade->setUnlocked(isUnlocked);
                    break;
                }
            }
        }
    }

    // Load wave manager data
    int currentWaveIndex;
    file.read(reinterpret_cast<char*>(&currentWaveIndex), sizeof(currentWaveIndex));
    wave->setCurrentWaveIndex(currentWaveIndex);

    size_t activeWaveCount;
    file.read(reinterpret_cast<char*>(&activeWaveCount), sizeof(activeWaveCount));

    for (size_t i = 0; i < activeWaveCount; ++i) {
        float enemyHealth;
        glm::vec2 enemyPosition;
        EnemyType enemyType;

        file.read(reinterpret_cast<char*>(&enemyHealth), sizeof(enemyHealth));
        file.read(reinterpret_cast<char*>(&enemyPosition), sizeof(enemyPosition));
        file.read(reinterpret_cast<char*>(&enemyType), sizeof(enemyType));

        Enemy enemy(enemyType, {enemyPosition.x, enemyPosition.y}, enemyHealth, 0.0f, 0.0f); // Add default values for damage and speed
        enemy.setPosition({enemyPosition.x, enemyPosition.y});
        wave->getCurrentEnemies()->push_back(enemy);
    }

    file.close();
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
            }),
        projectiles.end()
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

        if (island -> isLand(static_cast<int>((*enemies)[i].getPosition().at(0)) / Island::TILE_SIZE, static_cast<int>((*enemies)[i].getPosition().at(1)) / Island::TILE_SIZE)) {
            player -> hit((*enemies)[i].getDamage(), false);
            enemies -> erase(enemies -> begin() + i);
        }

        if ((*enemies)[i].getHealth() <= 0) {
            player -> takeCoins((*enemies)[i], 1.0f);
            enemies -> erase(enemies -> begin() + i);

            if (enemies -> empty()) {
                wave -> startNextWave();
                wave -> updateWaveStatus();
                enemies = wave -> getCurrentEnemies();
            }
        } else {
            ++i;
        }
    }

    Player::Movement();

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

    if (player -> getHealth() > 0) {
        player -> drawEntity(game -> texture);
    }

    if (wave) {
        for (const auto& enemy : *enemies) {
            if (enemy.getHealth() > 0) {
                enemy.drawEntity(game -> texture);
            }
        }
    }

    Player::drawLaser();

    turret -> render();
}
