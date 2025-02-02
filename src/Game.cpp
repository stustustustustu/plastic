#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height), window(NULL), shader(NULL), texture(NULL), renderer(NULL), manager(NULL), player({width/2, height/2}) {}

Game::~Game() {
    delete shader;
    delete texture;
    delete renderer;
    delete manager;
}

std::vector<float> Game::getSize() {
    return {width, height};
}

bool Game::Init() {
    if (!renderer -> initializeWindow(window, getSize().at(0), getSize().at(1), "plastic")) {
        return false;
    }

    // Create the shader program
    shader = new ShaderUtils(
        "../src/utils/shader/shaders/sprite/vertex.glsl",
        "../src/utils/shader/shaders/sprite/fragment.glsl"
    );

    renderer = new Renderer(*shader);
    manager = new WaveManager();


    texture = Texture::Create("../src/assets/sprites/sheet.png", true);

    if (!texture) {
        std::cerr << "ERROR: Failed to load texture!" << std::endl;
        return false;
    }

    texture -> Bind();

    // Spawn 3 turret objects
    turrets.push_back(Turret({width/2 - 100, height/2 - 100}, TurretType::LASER));
    turrets.push_back(Turret({width/2 + 100, height/2 - 100}, TurretType::RIFLE));
    turrets.push_back(Turret({width/2 + 100, height/2 + 100}, TurretType::BOMB));

    manager -> generateWaves(5, window);
    std::cout << "Starting wave 1." << std::endl;

    return true;
}

void Game::Update(float dt) {
    auto& enemies = manager -> getCurrentWave() -> getEnemies();

    Player::Movement(player, window, enemies); // Player movement

    for (auto& turret : turrets) { // Turret shooting
        turret.findTarget(enemies);
        turret.shoot();
    }

    for (int i = 0; i < enemies.size();) {
        enemies[i].moveTo(player);
        enemies[i].setSpeed(0.25f);

        if (enemies[i].getHealth() <= 0) {
            enemies.erase(enemies.begin() + i);
            if (enemies.empty()) {
                if (manager -> hasNextWave()) {
                    std::cout << "Starting wave " << manager -> getWaveIndex(*manager -> getCurrentWave()) + 2 << "." << std::endl;
                    manager -> startNextWave();
                    if (manager -> getCurrentWave()) {
                        enemies = manager -> getCurrentWave() -> getEnemies();
                    }
                } else {
                    std::cout << "All waves defeated!" << std::endl;
                    //glfwSetWindowShouldClose(window, true);
                }
            }
        } else {
            ++i;
        }
    }
}

void Game::Render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    renderer -> DrawSpriteSheet(*texture, glm::vec2(width/2, height/2), 0, 16, 16);

    if (player.getHealth() > 0) {
        player.drawEntity(*renderer, -1);
    }

    for (auto& turret : turrets) {
        turret.render(*renderer);
        turret.drawTargetLine(*renderer);
    }

    if (manager && manager -> getCurrentWave()) {
        const auto& enemies = manager -> getCurrentWave() -> getEnemies();
        for (const auto& enemy : enemies) {
            if (enemy.getHealth() > 0) {
                enemy.drawEntity(*renderer, 0);
            }
        }
    }

    glUseProgram(shader -> ID);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::Loop() {
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        Update(deltaTime);
        Render();
    }

    glDeleteProgram(shader -> ID);
    glfwDestroyWindow(window);
    glfwTerminate();
}