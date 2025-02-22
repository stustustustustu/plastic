#include "Renderer.h"
#include "../src/Game.h"

#include <../dependencies/glm/ext/matrix_clip_space.hpp>
#include <../dependencies/glm/ext/matrix_transform.hpp>

const auto game = Game::getInstance();

Renderer::Renderer(ShaderUtils &shader) : shader(shader), quadVAO(0), pixel(Texture::Create("../src/assets/sprites/pixel.png", true)), background(Texture::Create("../src/assets/sprites/background.png", true)){
	initRenderer();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::SetProjection(const glm::mat4 &projection) {
	this -> projection = projection;
}

glm::mat4 Renderer::GetProjection() const {
	return this -> projection;
}

void Renderer::initRenderer() {
	GLuint posVBO, texVBO;
	// pos VBO
	GLfloat posCoords[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f};

	// tex VBO
	GLfloat texCoords[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

    glGenVertexArrays(1, &this -> quadVAO);
	glBindVertexArray(this -> quadVAO);

    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posCoords), posCoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);

    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DrawText(const std::string &text, glm::vec2 position, int size, bool shadow, glm::vec3 color, TextAlignment alignment) const {
	if (shadow) {
		game -> text -> DrawText(text, position + glm::vec2(2, 2), size, HEXtoRGB(0x000000), alignment); // shadow
	}

	//DrawLine(glm::vec2(5, game -> text -> baseline), glm::vec2(game -> getSize().at(0) - 5, game -> text -> baseline), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)); // debug

	game -> text -> DrawText(text, position, size, color, alignment);
}

void Renderer::DrawSprite(Texture &texture, glm::vec2 position, float u1, float v1, float u2, float v2, glm::vec2 size, float rotate, glm::vec3 color) const {
	//Game::getInstance() -> batch -> pushObject(texture, {position, rotate}, {u1, v1, u2, v2}, size, {color, 1.0f});
	//return;

	this -> shader.Use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	this -> shader.SetMat4("model", model);
	this -> shader.SetMat4("projection", projection);
	this -> shader.SetVec3("spriteColor", color);

    GLfloat texCoords[] = {
        u1, v2,
        u2, v1,
        u1, v1,

        u1, v2,
        u2, v2,
        u2, v1,

    };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(this -> quadVAO);

	GLuint texVBO;
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &texVBO);
	glDisable(GL_BLEND);
}

void Renderer::DrawSprite(Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) const {
	this -> DrawSprite(texture, position, 0.0f, 0.0f, 1.0f, 1.0f, size, rotate, color);
}

void Renderer::DrawSpriteSheet(Texture &texture, glm::vec2 position, int index, int rows, int cols, glm::vec2 size, float rotate, glm::vec3 color) const {
	float u1 = static_cast<float>(index % cols) / cols;
	float v1 = static_cast<float>(index / cols) / rows;
	float u2 = static_cast<float>((index % cols) + 1) / cols;
	float v2 = static_cast<float>((index / cols) + 1) / rows;

	this -> DrawSprite(texture, position, u1, v1, u2, v2, size, rotate, color);
}

void Renderer::DrawBackground(int index) const {
	glm::vec2 position(0.0f, 0.0f);
	glm::vec2 size(1280.0f, 960.0f);

	DrawSpriteSheet(*background, position, index, 1, 4, size);
}

void Renderer::DrawLine(glm::vec2 position1, glm::vec2 position2, float thickness, glm::vec3 color) const {
	glm::vec2 pos = (position1 + position2) / 2.0f;
	glm::vec2 scale = glm::vec2(glm::distance(position1, position2), thickness);
	
	float angle = glm::degrees(glm::atan(position2.y - position1.y, position2.x - position1.x));
	pos -= scale / 2.0f;

	DrawSprite(*pixel, pos, scale, angle, color);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec3 color) const {
	DrawLine(glm::vec2(x1, y1), glm::vec2(x2, y2), thickness, color);
}

bool Renderer::initializeWindow(GLFWwindow*& window, int width, int height, const std::string& title) {
    if (!glfwInit()) {
        std::cout << "GLFW couldn't initialize." << std::endl;
        return false;
    }

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        std::cout << "GLFW couldn't create a window." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD couldn't initialize." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

	glViewport(0, 0, width, height);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    return true;
}