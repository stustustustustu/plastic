#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "core/UI/elements/toggle/Toggle.h"

Game *Game::instance = NULL;

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height),
                                        window(NULL), shader(NULL), texture(NULL), renderer(NULL), batch(NULL), text(NULL),
                                        turret(new TurretManager()),
                                        camera(new Camera(width, height)),
                                        input(NULL), wave(NULL), upgrade(NULL), inventory(NULL),
                                        generator(NULL),
                                        player(NULL) {}

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
    text = new TextRenderer(*t_shader, "../src/assets/font/ThaleahFat.ttf" , 200);

    input = new InputHandler();
    wave = new WaveManager();
    inventory = new Inventory();
    upgrade = new UpgradeManager(*inventory);
    generator = new Island(std::pow(rand(), 2));

    player = new Player();

    turret = new TurretManager();

    texture = Texture::Create("../src/assets/sprites/sheet.png", true);

    renderer -> SetProjection(camera -> getStaticProjection());
    camera -> returnToDefault();

    texture -> Bind();

    wave -> startNextWave();

    turret -> placeTurret(TurretType::LASER, {width/2 - 100, height/2 - 100});
    turret -> placeTurret(TurretType::RIFLE, {width/2 + 100, height/2 - 100});
    turret -> placeTurret(TurretType::RIFLE, {width/2 - 100, height/2 + 100});
    turret -> placeTurret(TurretType::BOMB, {width/2 + 100, height/2 + 100});

    return true;
}

Button *button = new Button(glm::vec2(200, 200), glm::vec2(100, 30), "Sigma");
Toggle *toggle = new Toggle(glm::vec2(200, 250), glm::vec2(100, 30));

void Game::Update() {
    enemies = wave -> getCurrentEnemies();

    button -> update();
    toggle -> update();

    input -> processInput();
    camera -> Update();

    Player::Movement(); // Player movement

    turret -> update();

    for (int i = 0; i < enemies -> size();) {
        (*enemies)[i].moveTowards(player -> getPosition());

        if (generator -> isLand(static_cast<int>((*enemies)[i].getPosition().at(0)) / Island::TILE_SIZE, static_cast<int>((*enemies)[i].getPosition().at(1)) / Island::TILE_SIZE)) {
            player -> hit((*enemies)[i].getDamage(), false);
            std::cout << "Hit for: " << (*enemies)[i].getDamage() << "." << std::endl;
            enemies -> erase(enemies -> begin() + i);
        }

        if ((*enemies)[i].getHealth() <= 0) {
            player -> takeCoins((*enemies)[i], 1.0f);
            enemies -> erase(enemies -> begin() + i);

            if (enemies -> empty()) {
                std::cout << "Player has " << player -> getHealth() << " health and " << player -> getCoins() << " coins." << std::endl;
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

    button -> render();
    toggle -> render();

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

    renderer -> SetProjection(camera -> getStaticProjection());

    renderer -> DrawText("ENEMIES " + std::to_string(enemies -> size()), glm::vec2(5, 55), 50.0f);

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