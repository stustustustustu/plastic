#include "World.h"
#include "../../Game.h"

const auto game = Game::getInstance();

World::World(unsigned int seed, Difficulty difficulty) : seed(seed), difficulty(difficulty) {}

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
    // save to file
}

void World::load(const std::string &path) {
    // load from file
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
