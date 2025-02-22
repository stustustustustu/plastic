#ifndef RENDERER_H
#define RENDERER_H

#include "../../config.h"
#include "sprites/Texture.h"
#include "../shader/ShaderUtils.h"
#include "text/TextRenderer.h"

class Renderer {
    private:
        void initRenderer();
        ShaderUtils &shader;
        unsigned int quadVAO;

        Texture *pixel;
        Texture *background;

        glm::mat4 projection;

    public:
        Renderer(ShaderUtils &shader);
        ~Renderer();

        void SetProjection(const glm::mat4 &projection);
        glm::mat4 GetProjection() const;

        void DrawText(
            const std::string &text,
            glm::vec2 position,
            int size,
            bool shadow,
            glm::vec3 color = glm::vec3(1.0f),
            TextAlignment alignment = TextAlignment::LEFT
        ) const;

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
        ) const;

        void DrawSpriteSheet(
            Texture &texture,
            glm::vec2 position,
            int index, int rows, int cols,
            glm::vec2 size = glm::vec2(32.0f),
            float rotate = 0.0f,
            glm::vec3 color = glm::vec3(1.0f)
        ) const;

        void DrawBackground(int index) const;

        void DrawLine(glm::vec2 position1, glm::vec2 position2, float thickness, glm::vec3 color) const;
        void DrawLine(float x1, float y1, float x2, float y2, float thickness, glm::vec3 color) const;

        bool initializeWindow(GLFWwindow*& window, int width, int height, const std::string& title);
};

#endif