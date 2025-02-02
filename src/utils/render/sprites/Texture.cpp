#include "Texture.h"

Texture::Texture(): ID(0), Width(0), Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), WrapS(GL_CLAMP_TO_EDGE), WrapT(GL_CLAMP_TO_EDGE), FilterMin(GL_NEAREST), FilterMax(GL_NEAREST) {
    glGenTextures(1, &this -> ID);
}

Texture::~Texture() {
    if (this -> ID != 0) {
        glDeleteTextures(1, &this -> ID);
    }
}

void Texture::Generate(unsigned int width, unsigned int height, unsigned char* data) {
    this -> Width = width;
    this -> Height = height;

    glBindTexture(GL_TEXTURE_2D, this -> ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this -> InternalFormat, width, height, 0, this -> ImageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this -> WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this -> WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this -> FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this -> FilterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture* Texture::Create(const char *file, bool alpha) {
    auto texture = new Texture;

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    if (alpha) {
        texture -> InternalFormat = GL_RGBA;
        texture -> ImageFormat = GL_RGBA;
    }

    if (!data) {
        std::cerr << "Failed to load texture: " << file << ". Error: " << stbi_failure_reason() << std::endl;
        delete texture;
        return nullptr;
    }

    texture -> Generate(width, height, data);

    stbi_image_free(data);
    return texture;
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
