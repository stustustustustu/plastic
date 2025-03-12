#include "World.h"

#include <string.h>

#include "../../Game.h"

const auto game = Game::getInstance();

World::World(std::string name, unsigned int seed, Difficulty difficulty) : name(name), seed(seed), difficulty(difficulty), enemies(new std::vector<Enemy>()) {}

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
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save world: " << path << std::endl;
        return;
    }

    // Save the length of the name first
    size_t nameLength = name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));

    // Save the name as a sequence of characters
    file.write(name.c_str(), nameLength);

    // Save seed, difficulty, and wave index
    file.write(reinterpret_cast<const char*>(&seed), sizeof(seed));
    file.write(reinterpret_cast<const char*>(&difficulty), sizeof(difficulty));
    int currentWaveIndex = wave->getCurrentWaveIndex();
    file.write(reinterpret_cast<const char*>(&currentWaveIndex), sizeof(currentWaveIndex));

    // Save the current wave's state (enemies)
    size_t enemyCount = enemies->size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));

    for (const auto& enemy : *enemies) {
        // Save enemy type
        EnemyType type = enemy.getType();
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));

        // Save enemy position
        glm::vec2 position = glm::vec2(enemy.getPosition().at(0), enemy.getPosition().at(1));
        file.write(reinterpret_cast<const char*>(&position), sizeof(position));

        // Save enemy health
        float health = enemy.getHealth();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
    }

    // Save window size
    auto windowSize = game->getSize();
    file.write(reinterpret_cast<const char*>(&windowSize[0]), sizeof(windowSize[0]));
    file.write(reinterpret_cast<const char*>(&windowSize[1]), sizeof(windowSize[1]));

    // Save player data (unchanged)
    float playerHealth = player->getHealth();
    float playerMaxHealth = player->getMaxHealth();
    float playerShield = player->getShield();
    int playerCoins = player->getCoins();
    glm::vec2 playerPosition = glm::vec2(player->getPosition().at(0), player->getPosition().at(1));

    file.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    file.write(reinterpret_cast<const char*>(&playerMaxHealth), sizeof(playerMaxHealth));
    file.write(reinterpret_cast<const char*>(&playerShield), sizeof(playerShield));
    file.write(reinterpret_cast<const char*>(&playerCoins), sizeof(playerCoins));
    file.write(reinterpret_cast<const char*>(&playerPosition), sizeof(playerPosition));

    // Save inventory data (unchanged)
    int coins = inventory->getCoins();
    file.write(reinterpret_cast<const char*>(&coins), sizeof(coins));

    // Save player upgrades (unchanged)
    size_t upgradeCount = upgrade->getPaths().size();
    file.write(reinterpret_cast<const char*>(&upgradeCount), sizeof(upgradeCount));

    for (const auto& [type, path] : upgrade->getPaths()) {
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        int level = path.getLevel();
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
    }

    // Save turret data (unchanged)
    size_t turretCount = turret->getTurrets().size();
    file.write(reinterpret_cast<const char*>(&turretCount), sizeof(turretCount));

    for (const auto& turretInstance : turret->getTurrets()) {
        TurretType type = turretInstance->getType();
        glm::vec2 position = glm::vec2(turretInstance->getPosition().at(0), turretInstance->getPosition().at(1));

        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&position), sizeof(position));
    }

    file.close();
}

void World::load(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to load world: " << path << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return;
    }

    // Load the length of the name first
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

    // Read the name as a sequence of characters
    std::string loadedName(nameLength, '\0');
    file.read(&loadedName[0], nameLength);
    name = loadedName;

    // Load seed, difficulty, and wave index
    file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
    int currentWaveIndex;
    file.read(reinterpret_cast<char*>(&currentWaveIndex), sizeof(currentWaveIndex));

    // Load the current wave's state (enemies)
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));

    // Clear existing enemies
    enemies->clear();

    for (size_t i = 0; i < enemyCount; ++i) {
        // Load enemy type
        EnemyType type;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));

        // Load enemy position
        glm::vec2 position;
        file.read(reinterpret_cast<char*>(&position), sizeof(position));

        // Load enemy health
        float health;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));

        // Get damage and speed from EnemyType
        auto enemyData = Enemy::getEnemyData();
        auto data = enemyData[type];
        float damage = std::get<2>(data); // Damage is the 3rd value in the tuple
        float speed = std::get<3>(data); // Speed is the 4th value in the tuple

        // Create the enemy
        Enemy enemy(type, {position.x, position.y}, health, damage, speed);

        // Add the enemy to the list
        enemies->push_back(enemy);
    }

    // Load window size
    float windowWidth, windowHeight;
    file.read(reinterpret_cast<char*>(&windowWidth), sizeof(windowWidth));
    file.read(reinterpret_cast<char*>(&windowHeight), sizeof(windowHeight));

    // Set the window size in the Game instance
    game->setSize(std::to_string(windowWidth) + "x" + std::to_string(windowHeight));

    // Reinitialize the world (but do not start a new wave)
    init();

    // Set the wave index to the saved value
    wave->setCurrentWaveIndex(currentWaveIndex);

    // Load player data
    float playerHealth;
    float playerMaxHealth;
    float playerShield;
    int playerCoins;
    glm::vec2 playerPosition;

    file.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    file.read(reinterpret_cast<char*>(&playerMaxHealth), sizeof(playerMaxHealth));
    file.read(reinterpret_cast<char*>(&playerShield), sizeof(playerShield));
    file.read(reinterpret_cast<char*>(&playerCoins), sizeof(playerCoins));
    file.read(reinterpret_cast<char*>(&playerPosition), sizeof(playerPosition));

    player->setHealth(playerHealth);
    player->setMaxHealth(playerMaxHealth);
    player->setShield(playerShield);
    player->setPosition({playerPosition.x, playerPosition.y});

    // Load inventory data
    int coins;
    file.read(reinterpret_cast<char*>(&coins), sizeof(coins));
    player->setCoins(coins); // Directly set the coins

    // Load player upgrades
    size_t upgradeCount;
    file.read(reinterpret_cast<char*>(&upgradeCount), sizeof(upgradeCount));

    for (size_t i = 0; i < upgradeCount; ++i) {
        UpgradeType type;
        int level;

        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&level), sizeof(level));

        // Ensure the upgrade path exists
        if (upgrade->hasUpgradePath(type)) {
            // Restore the level of the upgrade path
            upgrade->getPaths().at(type).restoreLevel(level);
        } else {
            // If the upgrade path doesn't exist, create it and set the level
            upgrade->addPath(type);
            upgrade->getPaths().at(type).restoreLevel(level);
        }

        // Apply the upgrade to the player's stats
        float multiplier = upgrade->getMultiplier(type);
        switch (type) {
            case UpgradeType::DAMAGE:
                player->setDamage(player->getDamage() * multiplier);
                break;
            case UpgradeType::HEALTH:
                player->setMaxHealth(player->getMaxHealth() * multiplier);
                player->setHealth(player->getMaxHealth());
                break;
            case UpgradeType::SPEED:
                player->setSpeed(player->getSpeed() * multiplier);
                break;
            case UpgradeType::SHIELD:
                player->setShield(player->getShield() * multiplier);
                player->setMaxShield(player->getMaxShield() * multiplier);
                break;
            default:
                break;
        }
    }

    // Load turret data
    size_t turretCount;
    file.read(reinterpret_cast<char*>(&turretCount), sizeof(turretCount));

    for (size_t i = 0; i < turretCount; ++i) {
        TurretType type;
        glm::vec2 position;

        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&position), sizeof(position));

        turret->addTurret(type, {position.x, position.y});
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
