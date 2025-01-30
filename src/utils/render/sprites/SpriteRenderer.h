#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../src/config.h"
#include "Texture.h"
#include "../src/utils/shader/ShaderUtils.h"

class SpriteRenderer {
    public:
        SpriteRenderer(ShaderUtils &shader);
        ~SpriteRenderer();

        void DrawSprite(Texture &texture, glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f)) const;
    private:
        ShaderUtils shader;
        unsigned int quadVAO;

        void initRenderData();
    };

#endif

