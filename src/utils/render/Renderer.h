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

    public:
        Renderer(ShaderUtils &shader);
        ~Renderer();

        void DrawSprite(
            Texture &texture,
            glm::vec2 position,
            float u1, float v1, float u2, float v2,
            glm::vec2 size = glm::vec2(32.0f),
            float rotate = 0.0f,
            glm::vec3 color = glm::vec3(1.0f)
        ) const;

        void DrawSprite(
            Texture &texture,
            glm::vec2 position,
            glm::vec2 size = glm::vec2(32.0f),
            float rotate = 0.0f,
            glm::vec3 color = glm::vec3(1.0f)
        );

        void DrawSpriteSheet(
            Texture &texture,
            glm::vec2 position,
            int index, int rows, int cols,
            glm::vec2 size = glm::vec2(32.0f),
            float rotate = 0.0f,
            glm::vec3 color = glm::vec3(1.0f)
        );

        void DrawRectangle(int x, int y, int width, int height, int hex) const;
        void DrawLine(float x1, float y1, float x2, float y2, float thickness, int hex) const;

        bool initializeWindow(GLFWwindow*& window, int width, int height, const std::string& title);
};

#endif

