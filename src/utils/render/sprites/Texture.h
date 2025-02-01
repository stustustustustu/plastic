#ifndef TEXTURE_H
#define TEXTURE_H

#include "../src/config.h"
#include "../dependencies/stb/stb_image.h"

// from LearnOpenGL.com
class Texture {
    public:
        unsigned int ID;
        unsigned int Width, Height;
        unsigned int InternalFormat;
        unsigned int ImageFormat;
        unsigned int WrapS;
        unsigned int WrapT;
        unsigned int FilterMin;
        unsigned int FilterMax;

        Texture();
        void Generate(unsigned int width, unsigned int height, unsigned char* data);
        bool Create(const std::string& filePath, unsigned int wrapS = GL_REPEAT, unsigned int wrapT = GL_REPEAT, unsigned int filterMin = GL_LINEAR_MIPMAP_LINEAR, unsigned int filterMax = GL_LINEAR);
        void Bind() const;
        void Unbind() const;
        void Delete();
};

#endif