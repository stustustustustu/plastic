#ifndef ISLAND_H
#define ISLAND_H

#include <../src/config.h>
#include <noise/FastNoise.h>
#include "../src/utils/render/sprites/Texture.h"

class Island {
    private:
        static constexpr int TILE_SIZE = 32;
        int MAP_WIDTH;
        int MAP_HEIGHT;

        unsigned int seed;

        FastNoise noise;

        std::vector<std::vector<float>> noiseMap;
        std::vector<std::vector<int>> tileMap;
        std::vector<std::vector<int>> tileVariants;

        float simplexNoise(float x, float y);
        float gradientMask(float x, float y);

        void classifyTile(int x, int y, float treshold);
        int getTileVariant(int x, int y);
        void saveTileData(int x, int y, int variant);

    public:
        Island(unsigned int seed);

        void generate();

        void render(Texture& tileTexture);
};

#endif //ISLAND_H
