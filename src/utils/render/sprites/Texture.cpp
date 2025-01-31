#include "Texture.h"

Texture::Texture() : ID(0), Width(0), Height(0) {}

void Texture::Generate(const std::string &file, bool alpha) {
    glGenTextures(1, &ID);
    if (ID == 0) {
        std::cerr << "[ERROR] Failed to generate texture ID!" << std::endl;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(file.c_str(), &Width, &Height, 0, alpha ? 4 : 3);

    if (!data) {
        std::cerr << "ERROR: Failed to load texture: " << file << std::endl;
        std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, Width, Height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}
