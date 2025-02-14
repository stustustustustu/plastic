#include "TextRenderer.h"
#include "../src/Game.h"

#include <glm/ext/matrix_clip_space.hpp>

const auto game = Game::getInstance();

TextRenderer::TextRenderer(ShaderUtils &t_t_shader, const std::string &fontPath, int fontSize) : t_shader(t_t_shader), fontPath(fontPath), fontSize(fontSize) {
    InitTextRenderer();
    LoadFont();
}

TextRenderer::~TextRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TextRenderer::InitTextRenderer() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::LoadFont() {
    std::ifstream fontFile(fontPath, std::ios::binary);
    if (!fontFile) {
        std::cerr << "ERROR::STB_TRUETYPE: Failed to open font file" << std::endl;
        return;
    }

    fontFile.seekg(0, std::ios::end);
    std::streamsize size = fontFile.tellg();
    fontFile.seekg(0, std::ios::beg);

    std::vector<unsigned char> fontData(size);
    if (!fontFile.read((char*)fontData.data(), size)) {
        std::cerr << "ERROR::STB_TRUETYPE: Failed to read font file" << std::endl;
        return;
    }

    stbtt_fontinfo fontInfo;
    if (!stbtt_InitFont(&fontInfo, fontData.data(), 0)) {
        std::cerr << "ERROR::STB_TRUETYPE: Failed to init font" << std::endl;
        return;
    }

    float scale = stbtt_ScaleForPixelHeight(&fontInfo, fontSize);

    for (GLubyte c = 0; c < 128; c++) {
        int width, height, xoff, yoff;
        unsigned char* bitmap = stbtt_GetCodepointBitmap(&fontInfo, 0, scale, c, &width, &height, &xoff, &yoff);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int advanceWidth, leftBearing;
        stbtt_GetCodepointHMetrics(&fontInfo, c, &advanceWidth, &leftBearing);

        Character character = {
            texture,
            glm::ivec2(width, height),
            glm::ivec2(leftBearing, yoff),
            static_cast<GLuint>(advanceWidth * scale)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));

        stbtt_FreeBitmap(bitmap, nullptr);
    }
}

void TextRenderer::DrawText(const std::string &text, glm::vec2 position, int size, float rotate, glm::vec3 color) const {
    t_shader.Use();
    t_shader.SetVec3("textColor", color);
    t_shader.SetMat4("projection", game -> renderer -> GetProjection());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(VAO);

    float scale = static_cast<float>(size) / fontSize;

    for (auto c = text.begin(); c != text.end(); c++) {
        Character ch = Characters.at(*c);

        float xpos = position.x + ch.bearing.x * scale;
        float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 }, // Bottom-left
            { xpos,     ypos,       0.0, 0.0 }, // Top-left
            { xpos + w, ypos,       1.0, 0.0 }, // Top-right

            { xpos,     ypos + h,   0.0, 1.0 }, // Bottom-left
            { xpos + w, ypos,       1.0, 0.0 }, // Top-right
            { xpos + w, ypos + h,   1.0, 1.0 }  // Bottom-right
        };

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        position.x += (ch.advance + 1) * scale;
    }

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}