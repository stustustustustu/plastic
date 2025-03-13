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

    // name
    size_t nameLength = name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(name.c_str(), nameLength);

    // seed, difficulty, and wave index
    file.write(reinterpret_cast<const char*>(&seed), sizeof(seed));
    file.write(reinterpret_cast<const char*>(&difficulty), sizeof(difficulty));
    int currentWaveIndex = wave->getCurrentWaveIndex();
    file.write(reinterpret_cast<const char*>(&currentWaveIndex), sizeof(currentWaveIndex));

    // window size
    auto windowSize = game->getSize();
    file.write(reinterpret_cast<const char*>(&windowSize[0]), sizeof(windowSize[0]));
    file.write(reinterpret_cast<const char*>(&windowSize[1]), sizeof(windowSize[1]));

    // player data
    float playerHealth = player->getHealth();
    float playerMaxHealth = player->getMaxHealth();

    float playerShield = player->getShield();
    float playerMaxShield = player->getMaxShield();

    int playerCoins = player->getCoins();

    glm::vec2 playerPosition = glm::vec2(player->getPosition().at(0), player->getPosition().at(1));

    file.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    file.write(reinterpret_cast<const char*>(&playerMaxHealth), sizeof(playerMaxHealth));

    file.write(reinterpret_cast<const char*>(&playerShield), sizeof(playerShield));
    file.write(reinterpret_cast<const char*>(&playerMaxShield), sizeof(playerMaxShield));

    file.write(reinterpret_cast<const char*>(&playerCoins), sizeof(playerCoins));

    file.write(reinterpret_cast<const char*>(&playerPosition), sizeof(playerPosition));

    // player upgrades
    size_t upgradeCount = upgrade->getPaths().size();
    file.write(reinterpret_cast<const char*>(&upgradeCount), sizeof(upgradeCount));

    for (const auto& [type, path] : upgrade->getPaths()) {
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        int level = path.getLevel();
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
    }

    // turret data
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

    // name
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    std::string loadedName(nameLength, '\0');
    file.read(&loadedName[0], nameLength);
    name = loadedName;

    // seed, difficulty, and wave index
    file.read(reinterpret_cast<char*>(&seed), sizeof(seed));
    file.read(reinterpret_cast<char*>(&difficulty), sizeof(difficulty));
    int currentWaveIndex;
    file.read(reinterpret_cast<char*>(&currentWaveIndex), sizeof(currentWaveIndex));

    // window size
    float windowWidth, windowHeight;
    file.read(reinterpret_cast<char*>(&windowWidth), sizeof(windowWidth));
    file.read(reinterpret_cast<char*>(&windowHeight), sizeof(windowHeight));
    game->setSize(std::to_string(windowWidth) + "x" + std::to_string(windowHeight));

    init();

    wave->setCurrentWaveIndex(currentWaveIndex);

    // player data
    float playerHealth;
    float playerMaxHealth;

    float playerShield;
    float playerMaxShield;

    int playerCoins;

    glm::vec2 playerPosition;

    file.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    file.read(reinterpret_cast<char*>(&playerMaxHealth), sizeof(playerMaxHealth));

    file.read(reinterpret_cast<char*>(&playerShield), sizeof(playerShield));
    file.read(reinterpret_cast<char*>(&playerMaxShield), sizeof(playerMaxShield));

    file.read(reinterpret_cast<char*>(&playerCoins), sizeof(playerCoins));

    file.read(reinterpret_cast<char*>(&playerPosition), sizeof(playerPosition));

    player->setHealth(playerHealth);
    player->setMaxHealth(playerMaxHealth);

    player->setShield(playerShield);
    player->setMaxShield(playerMaxShield);

    player->setCoins(playerCoins);

    player->setPosition({playerPosition.x, playerPosition.y});

    // player upgrades
    size_t upgradeCount;
    file.read(reinterpret_cast<char*>(&upgradeCount), sizeof(upgradeCount));

    for (size_t i = 0; i < upgradeCount; ++i) {
        UpgradeType type;
        int level;

        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&level), sizeof(level));

        if (upgrade->hasUpgradePath(type)) {
            upgrade->getPaths().at(type).restoreLevel(level);
        } else {
            upgrade->addPath(type);
            upgrade->getPaths().at(type).restoreLevel(level);
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
