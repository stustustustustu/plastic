#ifndef TEXTURE_H
#define TEXTURE_H

#include "../src/config.h"

class Texture {
public:
    GLuint ID;
    int Width, Height;

    Texture();
    void Generate(const std::string &file, bool alpha);
    void Bind() const;
    void Unbind() const;
    void Delete();
};

#endif