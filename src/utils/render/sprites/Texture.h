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
        ~Texture();

        void Generate(unsigned int width, unsigned int height, unsigned char* data);
        static Texture* Create(const char *file, bool alpha);
        void Bind() const;
        void Unbind() const;
        void Delete();
};

#endif