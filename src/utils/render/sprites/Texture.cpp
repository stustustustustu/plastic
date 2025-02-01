#include "Texture.h"

Texture::Texture() : ID(0), Width(0), Height(0) {}

void Texture::Generate(unsigned int width, unsigned int height, unsigned char* data) {
    this -> Width = width;
    this -> Height = height;

    // Create Texture
    glGenTextures(1, &this->ID);
    glBindTexture(GL_TEXTURE_2D, this -> ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this -> InternalFormat, width, height, 0, this -> ImageFormat, GL_UNSIGNED_BYTE, data);

    // Set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this -> WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this -> WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this -> FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this -> FilterMax);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::Create(const std::string& filePath, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMax) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << ". Error: " << stbi_failure_reason() << std::endl;
        return false;
    }

    // Determine image format based on number of channels
    if (nrChannels == 4) {
        this->InternalFormat = GL_RGBA;
        this->ImageFormat = GL_RGBA;
    } else if (nrChannels == 3) {
        this->InternalFormat = GL_RGB;
        this->ImageFormat = GL_RGB;
    } else {
        std::cerr << "Unsupported image format: " << filePath << std::endl;
        stbi_image_free(data);
        return false;
    }

    // Set texture parameters
    this -> WrapS = wrapS;
    this -> WrapT = wrapT;
    this -> FilterMin = filterMin;
    this -> FilterMax = filterMax;

    // Generate the texture
    Generate(width, height, data);

    // Free the image data
    stbi_image_free(data);

    return true;
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this -> ID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &this -> ID);
}
