#ifndef ISLANDGENERATION_H
#define ISLANDGENERATION_H

#include <../src/config.h>
#include "../src/utils/math/noise/Perlin.h"
#include "../src/utils/render/Renderer.h"
#include "../src/utils/render/sprites/Texture.h"

enum TileType {
    WATER,
    SAND
};

class Island {
    private:
        int WIDTH, HEIGHT;
        float noiseMap[1280][1280 * 3/4];

        Perlin perlin;

    public:
        Island(int width, int height, int seed = 0);
        void generate();
        float getNoiseValue(int x, int y) const;
        TileType getTileType(int x, int y, float treshold = 0.5f) const;

        void renderIsland(Renderer* renderer, Texture* texture, float tileSize);

    private:
        void generateNoiseMap();
        void applyMask();
        void addSmallIslands();
        void normalizeMap();
        void printNoiseMap();
};

#endif //ISLANDGENERATION_H
