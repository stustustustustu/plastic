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
    unsigned int VBO;
    float vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DrawSprite(const Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) const {
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

    if (texture.ID == 0) {
        std::cerr << "WARNING: Attempting to draw a sprite with an uninitialized texture!" << std::endl;
        return;
    }

    shader.Use();

    // Set the sprite color
    shader.SetVec3("spriteColor", color);

    // Calculate transformation matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // Set the model matrix
    shader.SetMat4("model", model);


    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f); // Replace with actual projection matrix

    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);

    // Bind the texture and set the texture uniform
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    shader.SetTexture("texture1", 0);
    shader.SetBool("hasTexture", true); // Set the flag to indicate we have a texture

    glBindVertexArray(quadVAO);
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