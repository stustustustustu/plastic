#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "../src/config.h"
#include "../../shader/ShaderUtils.h"
#include "stb/stb_truetype.h"

struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class TextRenderer {
    private:
        ShaderUtils &t_shader;
        std::string fontPath;
        int fontSize;
        std::map<GLchar, Character> Characters;
        GLuint VAO, VBO;

    public:
        TextRenderer(ShaderUtils &t_shader, const std::string &fontPath, int fontSize);
        ~TextRenderer();

        void InitTextRenderer();

        void DrawText(
            const std::string &text,
            glm::vec2 position,
            int size,
            float rotate = 0.0f,
            glm::vec3 color = glm::vec3(1.0f)
        ) const;

    private:
        void LoadFont();
};



#endif //TEXTRENDERER_H
