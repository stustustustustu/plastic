#ifndef ISLAND_H
#define ISLAND_H

#include <../src/config.h>
#include <noise/FastNoise.h>
#include "../src/utils/render/sprites/Texture.h"

enum class Tile {
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
    REVERSE_TOP_RIGHT = 2,         // Reverse indented corner (top-right)
    REVERSE_TOP_LEFT = 2,          // Reverse indented corner (top-left)
    REVERSE_BOTTOM_RIGHT = 2,      // Reverse indented corner (bottom-right)
    REVERSE_BOTTOM_LEFT = 2,       // Reverse indented corner (bottom-left)
    TRANSITION_TOP_LEFT = 2,       // Transition tile with land only in top-left
    TRANSITION_TOP_RIGHT = 2,      // Transition tile with land only in top-right
    TRANSITION_BOTTOM_LEFT = 2,    // Transition tile with land only in bottom-left
    TRANSITION_BOTTOM_RIGHT = 2    // Transition tile with land only in bottom-right
};

class Island {
    public:
        static constexpr int TILE_SIZE = 32;

    private:
        int MAP_WIDTH;
        int MAP_HEIGHT;

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

        Tile getBeachTileVariant(int x, int y);

        bool isLand(int x, int y) const;

        bool isWater(int x, int y) const;
};

#endif // ISLAND_H