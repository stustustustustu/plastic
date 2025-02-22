#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "../src/config.h"
#include "../../shader/ShaderUtils.h"
#include "stb/stb_truetype.h"

enum TextAlignment {
    LEFT,
    CENTER,
    RIGHT
};

struct Character {
    GLuint textureID;       // Texture ID for the character
    glm::ivec2 size;        // Size of the character glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    GLuint advance;         // Offset to advance to the next character
};

class TextRenderer {
    private:
        stbtt_fontinfo fontInfo;

        ShaderUtils &t_shader;  // Shader used for rendering text
        std::string fontPath;   // Path to the font file
        int fontSize;           // Base font size
        std::map<GLchar, Character> Characters; // Map of characters and their textures
        GLuint VAO, VBO;        // OpenGL buffers

        // metrics
        float ascent;           // Distance from baseline to top of tallest character
        float descent;          // Distance from baseline to bottom of lowest character
        float lineGap;          // Additional spacing between lines

    public:
        float baseline;

    public:
        TextRenderer(ShaderUtils &t_shader, const std::string &fontPath, int fontSize);
        ~TextRenderer();

        void InitTextRenderer(); // Initialize OpenGL buffers
        void LoadFont();         // Load the font and generate textures for all characters

        void DrawText(
            const std::string &text,
            glm::vec2 position,
            float fontSize,
            glm::vec3 color = glm::vec3(1.0f),
            TextAlignment alignment = TextAlignment::LEFT
        );

        float GetWidth(const std::string &text, float fontSize) const; // Calculate the width of a string

    private:
        void LoadCharacter(stbtt_fontinfo &fontInfo, GLubyte c, float scale); // Load a single character
};

#endif // TEXTRENDERER_H