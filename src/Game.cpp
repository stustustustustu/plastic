#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

Game *Game::instance = NULL;

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height), window(NULL), shader(NULL), texture(NULL), renderer(NULL), batch(NULL), text(NULL), wave(NULL), upgrade(NULL), inventory(NULL), generator(NULL), player({width/2, height/2}) {}

Game::~Game() {
    delete shader;
    delete texture;
    delete renderer;
    delete batch;
    delete text;
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

    auto t_shader = new ShaderUtils(
        "../src/utils/shader/shaders/text/vertex.glsl",
        "../src/utils/shader/shaders/text/fragment.glsl"
    );

    renderer = new Renderer(*shader);
    batch = new BatchRenderer(100000);
    text = new TextRenderer(*t_shader, "../src/assets/font/ThaleahFat.ttf" , 32);

    wave = new WaveManager();
    inventory = new Inventory(player);
    upgrade = new UpgradeManager(*inventory);
    generator = new Island(std::pow(rand(), 2));

    texture = Texture::Create("../src/assets/sprites/sheet.png", true);

    if (!texture) {
        std::cerr << "ERROR: Failed to load texture!" << std::endl;
        return false;
    }

    glm::mat4 projection = glm::ortho(0.0f, getSize().at(0), getSize().at(1), 0.0f, -1.0f, 1.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            batch -> mMvp[i][j] = projection[i][j];
        }
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

    renderer -> DrawText("test", {50, 50}, 50.0f);

    generator -> render(*texture);

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
    batch -> flush();
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