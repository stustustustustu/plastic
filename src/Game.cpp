#include "Game.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "core/UI/scene/scenes/InGame.h"
#include "core/UI/scene/scenes/MainMenu.h"
#include "core/UI/scene/scenes/Settings.h"
#include "core/UI/scene/scenes/WorldCreation.h"

Game *Game::instance = NULL;

Game::Game(float width, float height) : state(ACTIVE), width(width), height(height),
                                        window(NULL), shader(NULL), texture(NULL), renderer(NULL), batch(NULL), text(NULL),
                                        camera(new Camera(width, height)),
                                        input(NULL), scenes(NULL) {
    srand(time(NULL));
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

World * Game::getCurrentWorld() const {
    return currentWorld.get();
}

void Game::createNewWorld(unsigned int seed, Difficulty difficulty) {
    currentWorld = std::make_unique<World>(seed, difficulty);
    currentWorld -> init();

    scenes -> addScene(SceneType::GAME, std::make_unique<InGame>());
}

void Game::loadWorld(const std::string &path) {
    currentWorld = std::make_unique<World>(0, MEDIUM);
    currentWorld -> load(path);
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
    scenes = new SceneManager();

    texture = Texture::Create("../assets/sprites/sheet.png", true);

    renderer -> SetProjection(camera -> getStaticProjection());
    camera -> returnToDefault();

    texture -> Bind();

    scenes -> addScene(SceneType::MENU, std::make_unique<MainMenu>());
    scenes -> addScene(SceneType::WORLD_CREATION, std::make_unique<WorldCreation>());
    scenes -> addScene(SceneType::SETTINGS, std::make_unique<Settings>());

    scenes -> switchScene(SceneType::MENU);

    return true;
}

void Game::Update() {
    input -> processInput();
    camera -> Update();
    scenes -> update();

    if (currentWorld && scenes -> getScene() == SceneType::GAME) {
        getCurrentWorld() -> update();
    }
}

void Game::Render() const {
    glClear(GL_COLOR_BUFFER_BIT);

    if (currentWorld && scenes -> getScene() == SceneType::GAME) {
        getCurrentWorld() -> render();
    }

    scenes -> render();

    glfwSwapBuffers(window);
    glfwPollEvents();
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