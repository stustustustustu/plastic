#include "Texture.h"

Texture::Texture() : ID(0), Width(0), Height(0) {}

void Texture::Generate(const std::string &file, bool alpha) {
    glGenTextures(1, &ID);
    if (ID == 0) {
        std::cerr << "[ERROR] Failed to generate texture ID!" << std::endl;
    }
    std::cout << "[DEBUG] Generated Texture: ID=" << ID << std::endl;

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int channels;
    unsigned char* data = stbi_load(file.c_str(), &Width, &Height, &channels, alpha ? 4 : 3);

    if (!data) {
        std::cerr << "ERROR: Failed to load texture: " << file << std::endl;
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, Width, Height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Texture::Bind() const {
    if (ID == 0) {
        std::cerr << "WARNING: Attempted to bind a NULL texture!" << std::endl;
        return;
    }
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    if (ID != 0) {
        glDeleteTextures(1, &ID);
        ID = 0;
    }
}