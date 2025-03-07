#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "core/UI/scene/scenes/InGame.h"
#include "core/UI/scene/scenes/MainMenu.h"
#include "core/UI/scene/scenes/Settings.h"
#include "core/UI/scene/scenes/WorldCreation.h"

Game *Game::instance = NULL;

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height), difficulty(MEDIUM), seed(0),
                                        window(NULL), shader(NULL), texture(NULL), renderer(NULL), batch(NULL), text(NULL),
                                        turret(new TurretManager()),
                                        camera(new Camera(width, height)),
                                        input(NULL), wave(NULL), upgrade(NULL), inventory(NULL),
                                        generator(NULL), scenes(NULL),
                                        player(NULL), enemies(NULL) {
    srand(time(NULL));
    setSeed(std::pow(rand(), 2));
}

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

void Game::setSize(const std::string &size) {
    size_t x = size.find('x');
    if (x != std::string::npos) {
        int w = std::stoi(size.substr(0, x));
        int h = std::stoi(size.substr(x + 1));

        glViewport(0, 0, w, h);

        resizeWindow(glm::ivec2(w, h));
    }
}

void Game::resizeWindow(glm::ivec2 size) {
    this -> width = size.x;
    this -> height = size.y;

    glfwSetWindowSize(window, size.x, size.y);

    camera -> setSize(size.x, size.y);

    scenes -> resize();
}

Difficulty Game::getDifficulty() const {
    return this -> difficulty;
}

void Game::setDifficulty(Difficulty difficulty) {
    this -> difficulty = difficulty;
}

unsigned int Game::getSeed() const {
    return this -> seed;
}

void Game::setSeed(unsigned int seed) {
    this -> seed = seed;
}

float Game::getVolume() const {
    return this -> volume;
}

void Game::setVolume(float volume) {
    this -> volume = volume;
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
    text = new TextRenderer(*t_shader, "../assets/font/quaver.ttf" , 48);

    input = new InputHandler();
    wave = new WaveManager();
    inventory = new Inventory();
    upgrade = new UpgradeManager(*inventory);
    generator = new Island(seed);
    scenes = new SceneManager();

    player = new Player();

    turret = new TurretManager();

    texture = Texture::Create("../assets/sprites/sheet.png", true);

    renderer -> SetProjection(camera -> getStaticProjection());
    camera -> returnToDefault();

    texture -> Bind();

    scenes -> addScene(SceneType::GAME, std::make_unique<InGame>());
    scenes -> addScene(SceneType::MENU, std::make_unique<MainMenu>());
    scenes -> addScene(SceneType::WORLD_CREATION, std::make_unique<WorldCreation>());
    scenes -> addScene(SceneType::SETTINGS, std::make_unique<Settings>());

    scenes -> switchScene(SceneType::MENU);

    wave -> startNextWave();

    return true;
}

void Game::Update() {
    enemies = wave -> getCurrentEnemies();

    input -> processInput();
    camera -> Update();
    scenes -> update();

    //std::cout << difficulty << std::endl;

    if (scenes -> getScene() == SceneType::GAME) {
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

        Player::Movement(); // player movement

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
}

void Game::Render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    renderer -> SetProjection(camera -> getCameraProjection());

    generator -> render(*texture);

    if (scenes -> getScene() == SceneType::GAME) {
        for (auto &projectile : projectiles) {
            projectile -> render();
        }

        for (auto &explosion : explosions) {
            explosion -> render();
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

        //renderer -> DrawText("ABCDEFGHIJKLMNOPQRSTUVWXYZ", glm::vec2(5, 50), 24.0f, true);
        //renderer -> DrawText("abcdefghijklmnopqrstuvwxyz", glm::vec2(5, 50 + 24), 24.0f, true);
        //renderer -> DrawText("0123456789", glm::vec2(5, 50 + 48), 24.0f, true);
        //renderer -> DrawText(".,:;_*+-/=!?()[]{}<>#$%&@", glm::vec2(5, 50 + 72), 24.0f, true);

        turret -> render();
    }

    renderer -> SetProjection(camera -> getStaticProjection());

    scenes -> render();

    glfwSwapBuffers(window);
    glfwPollEvents();
    //batch -> flush();
}

void Game::Loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (width != this -> width || height != this -> height) {
            resizeWindow(glm::ivec2(width, height));
        }

        Update();
        Render();
    }

    glDeleteProgram(shader -> ID);
    glfwDestroyWindow(window);
    glfwTerminate();
}