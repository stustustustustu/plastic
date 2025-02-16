#ifndef ISLAND_H
#define ISLAND_H

#include <../src/config.h>
#include <noise/FastNoise.h>
#include "../src/utils/render/sprites/Texture.h"

enum class Tile {
    EMPTY = 1000,                   // empty tile
    WATER = 3,                      // Water tile
    ISOLATED = 2,                   // Isolated land tile
    CENTER = 4,                     // Full land tile
    TOP = 5,                        // Land with water on top
    RIGHT = 6,                      // Land with water on the right
    BOTTOM = 7,                     // Land with water on the bottom
    LEFT = 8,                       // Land with water on the left
    TOP_RIGHT = 9,                  // Land with water on top and top-right
    BOTTOM_RIGHT = 10,              // Land with water on bottom and right
    BOTTOM_LEFT = 11,               // Land with water on bottom and left
    TOP_LEFT = 12,                  // Land with water on top and left
    REVERSE_TOP_RIGHT = 13,
    REVERSE_TOP_LEFT = 14,
    REVERSE_BOTTOM_RIGHT = 15,
    REVERSE_BOTTOM_LEFT = 16,
    DIAGONAL_TR_BL = 17,              // water op top right and bottom left
    DIAGONAL_TL_BR = 18              // water op top left and bottom right
};

class Island {
    public:
        static constexpr int TILE_SIZE = 32;
        int MAP_WIDTH;
        int MAP_HEIGHT;

    private:
        unsigned int seed;

        FastNoise noise;

        std::vector<std::vector<float>> noiseMap;
        std::vector<std::vector<int>> tileMap;
        std::vector<std::vector<Tile>> tileVariants;

        float simplexNoise(float x, float y);
        float gradientMask(float x, float y);

        void classifyTile(int x, int y, float threshold);
        Tile getTileVariant(int x, int y);

        void saveTileData(int x, int y, Tile variant);

    public:
        Island(unsigned int seed);

        void generate();

        void traceIslandEdge();

        void render(Texture& tileTexture);

        Tile getBeachTileVariant(int x, int y) const;

        std::pair<int, int> findNearestLandTile(float screenX, float screenY) const;

        float distanceToNearestLand(float worldX, float worldY) const;

        bool isLand(int x, int y) const;

        bool isWater(int x, int y) const;
};

#endif // ISLAND_H