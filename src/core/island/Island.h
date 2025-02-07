#ifndef ISLANDGENERATION_H
#define ISLANDGENERATION_H

#include <../src/config.h>
#include <stb/stb_perlin.h>
#include "../src/tilemap/Tilemap.h"

class Island {
private:
    int width, height;
    Tilemap tilemap;
    bool complete = false;

    int seed;
    float radius;

    std::mutex mx;
    std::condition_variable cv;
private:
    void applyBitmask();

public:
    Island(int width, int height, int seed, float radius) : tilemap(width, height), seed(seed), radius(radius) {
        generate(seed);
    }

    void generate(unsigned int seed);
    void render(Renderer* renderer, Texture* texture);
};


#endif //ISLANDGENERATION_H
