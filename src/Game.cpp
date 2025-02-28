#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "core/UI/scene/scenes/InGame.h"

Game *Game::instance = NULL;

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height),
                                        window(NULL), shader(NULL), texture(NULL), renderer(NULL), batch(NULL),
                                        text(NULL),
                                        turret(new TurretManager()),
                                        camera(new Camera(width, height)),
                                        input(NULL), wave(NULL), upgrade(NULL), inventory(NULL),
                                        generator(NULL), scenes(NULL),
                                        player(NULL), enemies(NULL) {}

Game::~Game() {
    delete shader;
    delete texture;
    delete renderer;
    delete batch;
    delete text;

    delete turret;

    delete camera;

    delete player;
    delete input;
    delete wave;
    delete upgrade;
    delete scenes;

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
    text = new TextRenderer(*t_shader, "../src/assets/font/quaver.ttf" , 48);

    input = new InputHandler();
    wave = new WaveManager();
    inventory = new Inventory();
    upgrade = new UpgradeManager(*inventory);
    generator = new Island(std::pow(rand(), 2));
    scenes = new SceneManager();

    player = new Player();

    turret = new TurretManager();

    texture = Texture::Create("../src/assets/sprites/sheet.png", true);

    renderer -> SetProjection(camera -> getStaticProjection());
    camera -> returnToDefault();

    texture -> Bind();

    wave -> startNextWave();

    scenes -> addScene(SceneType::GAME, std::make_unique<InGame>());
    scenes -> switchScene(SceneType::GAME);

    return true;
}

void Game::Update() {
    enemies = wave -> getCurrentEnemies();

    input -> processInput();
    camera -> Update();
    scenes -> update();

    for (auto &projectile : projectiles) {
        projectile -> update();
    }

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const std::unique_ptr<Projectile>& projectile) {
                return projectile -> isMarked();
            }),
        projectiles.end()
    );

    Player::Movement(); // Player movement

    turret -> update();

    for (int i = 0; i < enemies -> size();) {
        (*enemies)[i].moveTowards(player -> getPosition());

        if (generator -> isLand(static_cast<int>((*enemies)[i].getPosition().at(0)) / Island::TILE_SIZE, static_cast<int>((*enemies)[i].getPosition().at(1)) / Island::TILE_SIZE)) {
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
}

void Game::Render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    renderer -> SetProjection(camera -> getCameraProjection());

    generator -> render(*texture);

    for (auto &projectile : projectiles) {
        projectile->render();
    }

    if (player -> getHealth() > 0) {
        player -> drawEntity(texture);
    }

    if (wave) {
        for (const auto& enemy : *enemies) {
            if (enemy.getHealth() > 0) {
                enemy.drawEntity(texture);
            }
        }
    }

    Player::drawLaser();

    renderer -> SetProjection(camera -> getStaticProjection());

    scenes -> render();
    //renderer -> DrawText("ABCDEFGHIJKLMNOPQRSTUVWXYZ", glm::vec2(5, 50), 24.0f, true);
    //renderer -> DrawText("abcdefghijklmnopqrstuvwxyz", glm::vec2(5, 50 + 24), 24.0f, true);
    //renderer -> DrawText("0123456789", glm::vec2(5, 50 + 48), 24.0f, true);
    //renderer -> DrawText(".,:;_*+-/=!?()[]{}<>#$%&@", glm::vec2(5, 50 + 72), 24.0f, true);

    turret -> render();

    glfwSwapBuffers(window);
    glfwPollEvents();
    //batch -> flush();
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