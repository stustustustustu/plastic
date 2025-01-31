#ifndef RENDERER_H
#define RENDERER_H

#include "../../config.h"
#include "sprites/Texture.h"
#include "../shader/ShaderUtils.h"

class Renderer {
    private:
        void initRenderData();
        ShaderUtils &shader;
        unsigned int quadVAO;
        glm::mat4 view;
    public:
        Renderer(ShaderUtils &shader);
        ~Renderer();

        void DrawSprite(const Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(1.0f, 1.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f)) const;

        void DrawRectangle(int x, int y, int width, int height, int hex) const;
        void DrawLine(float x1, float y1, float x2, float y2, float thickness, int hex) const;

        bool initializeWindow(GLFWwindow*& window, int width, int height, const std::string& title);
};

#endif

