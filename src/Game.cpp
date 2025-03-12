#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "core/UI/scene/scenes/InGame.h"
#include "core/UI/scene/scenes/MainMenu.h"
#include "core/UI/scene/scenes/Pause.h"
#include "core/UI/scene/scenes/Settings.h"
#include "core/UI/scene/scenes/WorldCreation.h"
#include "core/UI/scene/scenes/WorldSelection.h"

Game *Game::instance = NULL;

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height),
                                        window(NULL), shader(NULL), texture(NULL), renderer(NULL), batch(NULL), text(NULL),
                                        camera(new Camera(width, height)),
                                        input(NULL), scenes(NULL) {
    srand(time(NULL));

    if (!std::filesystem::exists("saves")) {
        std::filesystem::create_directory("saves");
    }
}

Game::~Game() {
    delete shader;
    delete texture;
    delete renderer;
    delete batch;
    delete text;

    delete camera;

    delete input;
    delete scenes;
}

Game *Game::getInstance(float width, float height) {
    if (!instance) {
        instance = new Game(width, height);
    }

    return instance;
}

World* Game::getCurrentWorld() const {
    return currentWorld.get();
}

void Game::createNewWorld(std::string name, unsigned int seed, Difficulty difficulty) {
    currentWorld = std::make_unique<World>(name, seed, difficulty);
    currentWorld -> init();

    scenes -> addScene("IN_GAME", std::make_unique<InGame>());
}

void Game::loadWorld(const std::string &path) {
    currentWorld = std::make_unique<World>("New World", 0, MEDIUM);
    currentWorld -> load(path);

    scenes->addScene("IN_GAME", std::make_unique<InGame>());
}

GameState Game::getState() const {
    return this -> state;
}

void Game::setState(GameState state) {
    this -> state = state;
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

float Game::getVolume() const {
    return this -> volume;
}

void Game::setVolume(float volume) {
    this -> volume = volume;
}

bool Game::init() {
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
    scenes = new SceneManager();

    texture = Texture::Create("../assets/sprites/sheet.png", true);

    renderer -> SetProjection(camera -> getStaticProjection());
    camera -> returnToDefault();

    texture -> Bind();

    scenes -> addScene("MAIN_MENU", std::make_unique<MainMenu>());
    scenes -> addScene("WORLD_CREATION", std::make_unique<WorldCreation>());
    scenes -> addScene("WORLD_SELECTION", std::make_unique<WorldSelection>());
    scenes -> addScene("SETTINGS", std::make_unique<Settings>());
    scenes -> addScene("PAUSE", std::make_unique<Pause>());

    scenes -> switchScene("MAIN_MENU");

    return true;
}

void Game::update() {
    input -> processInput();
    camera -> Update();
    scenes -> update();

    if (currentWorld && scenes -> getScene() == "IN_GAME") {
        getCurrentWorld() -> update();
    }
}

void Game::render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    if (currentWorld && (scenes -> getScene() == "IN_GAME" || scenes -> getScene() == "PAUSE")) {
        getCurrentWorld() -> render();
    }

    scenes -> render();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (width != this -> width || height != this -> height) {
            resizeWindow(glm::ivec2(width, height));
        }

        update();
        render();
    }

    glDeleteProgram(shader -> ID);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::handlePause() {
    if (state == GameState::ACTIVE) {
        scenes -> switchScene("PAUSE");
        state = GameState::PAUSED;
    } else if (state == GameState::PAUSED) {
        scenes -> goBack();
        state = GameState::ACTIVE;
    }
}
