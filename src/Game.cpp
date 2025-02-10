#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

Game *Game::instance = NULL;

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height), window(NULL), shader(NULL), texture(NULL), renderer(NULL), batch(NULL), wave(NULL), upgrade(NULL), inventory(NULL), generator(NULL), player({width/2, height/2}) {}

Game::~Game() {
    delete shader;
    delete texture;
    delete renderer;
    delete batch;
    delete wave;
    delete upgrade;
    delete inventory;
    delete generator;
}

Game *Game::getInstance(float width, float height) {
    if (!instance) {
        instance = new Game(width, height);
    }

    return instance;
}

std::vector<float> Game::getSize() {
    return {width, height};
}

bool Game::Init() {
    srand(time(NULL));

    if (!renderer -> initializeWindow(window, getSize().at(0), getSize().at(1), "plastic")) {
        return false;
    }

    shader = new ShaderUtils(
        "../src/utils/shader/shaders/sprite/vertex.glsl",
        "../src/utils/shader/shaders/sprite/fragment.glsl"
    );

    renderer = new Renderer(*shader);
    batch = new BatchRenderer(100000);
    wave = new WaveManager();
    inventory = new Inventory(player);
    upgrade = new UpgradeManager(*inventory);
    //generator = new Island(width / 32.0f, height / 32.0f, std::pow(rand(), 2), 50.0f);

    texture = Texture::Create("../src/assets/sprites/sheet.png", true);

    if (!texture) {
        std::cerr << "ERROR: Failed to load texture!" << std::endl;
        return false;
    }

    texture -> Bind();

    turrets.push_back(Turret({width/2 - 100, height/2 - 100}, TurretType::LASER));
    turrets.push_back(Turret({width/2 + 100, height/2 - 100}, TurretType::RIFLE));
    turrets.push_back(Turret({width/2 + 100, height/2 + 100}, TurretType::BOMB));

    wave -> startNextWave();

    return true;
}

void Game::Update() {
    enemies = wave -> getCurrentEnemies();

    Player::Movement(); // Player movement

    for (auto& turret : turrets) { // Turret shooting
        turret.findTarget();
        turret.shoot();
    }

    for (int i = 0; i < enemies -> size();) {
        (*enemies)[i].moveTowards(player);
        (*enemies)[i].setSpeed(0.25f);

        if ((*enemies)[i].getHealth() <= 0) {
            player.takeCoins((*enemies)[i], 1.0f);
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
}

void Game::Render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    static int currentFrame = 0;
    static std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime).count();

    if (elapsedTime >= 400) {
        currentFrame = (currentFrame + 1) % 4;
        lastUpdateTime = currentTime;
    }

    renderer -> DrawBackground(currentFrame);

    //generator -> render(renderer, texture);

    if (player.getHealth() > 0) {
        player.drawEntity(texture);
    }

    for (auto& turret : turrets) {
        turret.render(texture);
    }

    if (wave) {
        for (const auto& enemy : *enemies) {
            if (enemy.getHealth() > 0) {
                enemy.drawEntity(texture);
            }
        }
    }

    glUseProgram(shader -> ID);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::Loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        Update();
        Render();
    }

    glDeleteProgram(shader -> ID);
    glfwDestroyWindow(window);
    glfwTerminate();
}