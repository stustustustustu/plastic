#include "Game.h"

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height), window(NULL), shader(NULL), texture(NULL), renderer(NULL), player({width/2, height/2}) {
}

Game::~Game() {
    delete shader;
    delete texture;
    delete renderer;
}

std::vector<float> Game::getSize() {
    return {width, height};
}

bool Game::Init() {
    if (!initializeWindow(window, getSize().at(0), getSize().at(1), "plastic")) {
        return false;
    }

    // Create the shader program
    shader = new ShaderUtils(
        "../src/utils/shader/shaders/vertex.txt",
        "../src/utils/shader/shaders/fragment.txt"
    );

    Texture texture;
    texture.Generate("../src/assets/sprites/sheet.png", true);

    renderer = new SpriteRenderer(*shader);


    // Spawn 3 turret objects
    turrets.push_back(Turret({width/2 - 100, height/2 - 100}, TurretType::LASER));
    turrets.push_back(Turret({width/2 + 100, height/2 - 100}, TurretType::RIFLE));
    turrets.push_back(Turret({width/2 + 100, height/2 + 100}, TurretType::BOMB));

    Entity::populate(enemies, 5, window);

    return true;
}

void Game::Update(float dt) {
    Player::Movement(player, window, enemies); // player movement

    for (auto& turret : turrets) { // turret shooting
        turret.findTarget(enemies);
        turret.shoot();
    }

    for (int i = 0; i < enemies.size(); i++) {
        // enemy movement
        enemies[i].moveTo(player);
        enemies[i].setSpeed(0.25f);

        if (enemies[i].getHealth() <= 0) {
            enemies.erase(enemies.begin() + i);
            if (enemies.empty()) {
                Entity::populate(enemies, 5, window);
            }
        }
    }
}

void Game::Render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    //renderer -> DrawSprite(*texture, glm::vec2(10.0f, 10.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    if (player.getHealth() > 0) {
        player.drawEntity(-1);
    }

    for (auto& turret : turrets) {
        turret.render();
        turret.drawTargetLine();
    }

    for (auto& enemy : enemies) {
        if (enemy.getHealth() > 0) {
            enemy.drawEntity(0);
        }
    }

    glfwSwapBuffers(window);
    glUseProgram(shader -> ID);
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