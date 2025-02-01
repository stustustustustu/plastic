#include "Renderer.h"

#include <../dependencies/glm/ext/matrix_clip_space.hpp>
#include <../dependencies//glm/ext/matrix_transform.hpp>

Renderer::Renderer(ShaderUtils &shader) : shader(shader), quadVAO(0) {
    initRenderData();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::initRenderData() {
    GLuint posVBO, texVBO;
    // Pos VBO
    GLfloat positions[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f};

    // Texture VBO
    GLfloat texCoords[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f};

    glGenVertexArrays(1, &this -> quadVAO);
    glBindVertexArray(this -> quadVAO);

    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid *)0);

    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DrawSprite(Texture &texture, glm::vec2 position, float u1, float v1, float u2, float v2, glm::vec2 size, float rotate, glm::vec3 color) {
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

    this -> shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    this -> shader.SetBool("useTexture", true);
    this -> shader.SetMat4("model", model);
    this -> shader.SetVec3("spriteColor", color);
    this -> shader.SetBool("useMask", false);

    // clang-format off
    GLfloat texCoords[] = {
        u1, v2,
        u2, v1,
        u1, v1,

        u1, v2,
        u2, v2,
        u2, v1,
    };
    // clang-format on

    glBindVertexArray(this -> quadVAO);

    GLuint texVBO;
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &texVBO);
}

void Renderer::DrawSprite(Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    this -> DrawSprite(texture, position, 0.0f, 0.0f, 1.0f, 1.0f, size, rotate, color);
}

void Renderer::DrawSpriteSheet(Texture &texture, glm::vec2 position, int index, int rows, int cols, glm::vec2 size, float rotate, glm::vec3 color) {
    float u1 = (index % cols) / (float)cols;
    float v1 = (index / cols) / (float)rows;
    float u2 = (index % cols + 1) / (float)cols;
    float v2 = (index / cols + 1) / (float)rows;

    this -> DrawSprite(texture, position, u1, v1, u2, v2, size, rotate, color);
}

void Renderer::DrawSpriteWithMask(Texture &spriteTexture, Texture &maskTexture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    this -> shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    this -> shader.SetBool("useTexture", true);
    this -> shader.SetMat4("model", model);
    this -> shader.SetVec3("spriteColor", color);
    this -> shader.SetBool("useMask", true);

    glActiveTexture(GL_TEXTURE0);
    spriteTexture.Bind();
    this -> shader.SetInt("spriteTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    maskTexture.Bind();
    this -> shader.SetInt("maskTexture", 1);

    glBindVertexArray(this -> quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::DrawRectangle(int x, int y, int width, int height, int hex) const {
    // Get the current framebuffer size (screen size)
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

    // Normalize the screen coordinates to OpenGL's NDC (Normalized Device Coordinates)
    float normalizedX = (2.0f * x / screenWidth) - 1.0f;   // x maps from [0, screenWidth] -> [-1, 1]
    float normalizedY = 1.0f - (2.0f * y / screenHeight);  // y maps from [0, screenHeight] -> [1, -1]
    float normalizedWidth = (2.0f * width / screenWidth);   // width maps from [0, screenWidth] -> [0, 2]
    float normalizedHeight = (2.0f * height / screenHeight); // height maps from [0, screenHeight] -> [0, 2]

    // Extract the RGB components from the hex color (0xRRGGBB)
    float red   = ((hex >> 16) & 0xFF) / 255.0f;   // Extract red (bits 16-23) and normalize to [0, 1]
    float green = ((hex >> 8) & 0xFF) / 255.0f;    // Extract green (bits 8-15) and normalize to [0, 1]
    float blue  = (hex & 0xFF) / 255.0f;           // Extract blue (bits 0-7) and normalize to [0, 1]

    // Define the vertices for the rectangle in NDC coordinates
    float vertices[] = {
        // Positions              // Colors
        normalizedX - normalizedWidth / 2, normalizedY - normalizedHeight / 2, 0.0f, red, green, blue,  // Bottom left
        normalizedX + normalizedWidth / 2, normalizedY - normalizedHeight / 2, 0.0f, red, green, blue,  // Bottom right
        normalizedX + normalizedWidth / 2, normalizedY + normalizedHeight / 2, 0.0f, red, green, blue,  // Top right
        normalizedX - normalizedWidth / 2, normalizedY + normalizedHeight / 2, 0.0f, red, green, blue   // Top left
    };

    unsigned int indices[] = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.Use();

    shader.SetBool("useTexture", false);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, float thickness, int hex) const {
    // Get the current framebuffer size (screen size)
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

    // Normalize the screen coordinates to OpenGL's NDC (Normalized Device Coordinates)
    float normalizedX1 = (2.0f * x1 / screenWidth) - 1.0f;   // x1 maps from [0, screenWidth] -> [-1, 1]
    float normalizedY1 = 1.0f - (2.0f * y1 / screenHeight);  // y1 maps from [0, screenHeight] -> [1, -1]

    float normalizedX2 = (2.0f * x2 / screenWidth) - 1.0f;   // x2 maps from [0, screenWidth] -> [-1, 1]
    float normalizedY2 = 1.0f - (2.0f * y2 / screenHeight);  // y2 maps from [0, screenHeight] -> [1, -1]

    // Extract the RGB components from the hex color (0xRRGGBB)
    float red   = ((hex >> 16) & 0xFF) / 255.0f;   // Extract red (bits 16-23) and normalize to [0, 1]
    float green = ((hex >> 8) & 0xFF) / 255.0f;    // Extract green (bits 8-15) and normalize to [0, 1]
    float blue  = (hex & 0xFF) / 255.0f;           // Extract blue (bits 0-7) and normalize to [0, 1]

    // Define the vertices for the line in NDC coordinates
    float vertices[] = {
        normalizedX1, normalizedY1, 0.0f, red, green, blue,  // Start point
        normalizedX2, normalizedY2, 0.0f, red, green, blue   // End point
    };

    unsigned int VBO, VAO;
    glLineWidth(thickness);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.Use();

    shader.SetBool("useTexture", false);

    // Draw the line
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    return true;
}