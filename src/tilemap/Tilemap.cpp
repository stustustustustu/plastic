#include "Tilemap.h"

#include <stb/stb_perlin.h>

Tile Tilemap::getTile(glm::vec2 pos) const {
    if (pos.x >= 0 && pos.x < tiles[0].size() && pos.y >= 0 && pos.y < tiles.size()) {
        return tiles[pos.y][pos.x];
    }
    return Tile();
}

void Tilemap::setTile(glm::vec2 pos, const Tile &tile) {
    if (pos.x >= 0 && pos.x < tiles[0].size() && pos.y >= 0 && pos.y < tiles.size()) {
        tiles[pos.y][pos.x] = tile;
    }
}

float Tilemap::calculateAvgNoise(glm::vec2 startPos, int quadSize, int seed) {
    float total = 0.0f;
    int count = 0;

    for (int y = startPos.y; y < startPos.y + quadSize; ++y) {
        for (int x = startPos.x; x < startPos.x + quadSize; ++x) {
            float noiseValue = stb_perlin_noise3(
                (x + seed) * 0.05f,             // Scale X
                (y + seed) * 0.05f,             // Scale Y
                0.0f,                           // Z (constant for 2D)
                1.0f, 1.0f, 1.0f  // Frequency
            );
            total += noiseValue;
            count++;
        }
    }

    return total / count;
}

int Tilemap::getBitmask(glm::vec2 pos, TileType type) const {
    int bitmask = 0;

    if (pos.y > 0 && this -> getTile(glm::vec2(pos.x, pos.y - 1)).type == type) bitmask |= 1; // top
    if (pos.x > width && this -> getTile(glm::vec2(pos.x + 1, pos.y)).type == type) bitmask |= 2; // right
    if (pos.y < height - 1 && this -> getTile(glm::vec2(pos.x, pos.y + 1)).type == type) bitmask |= 4; // bottom
    if (pos.x > 0 && this -> getTile(glm::vec2(pos.x - 1, pos.y)).type == type) bitmask |= 8; // left

    return bitmask;
}

int Tilemap::getVariant(int bitmask, TileType type) {
    static const std::map<int, int> sandVariants = {
        {0b0000, 0}, // isolated
        {0b0001, 1}, // top
        {0b0010, 2}, // right
        {0b0100, 3}, // bottom
        {0b1000, 4}, // left
        {0b0101, 5}, // vertical
        {0b0011, 6}, // horizontal
        {0b1111, 7}  // surrounded
    };

    if (type == SAND) {
        auto it = sandVariants.find(bitmask);

        return it != sandVariants.end() ? it -> second : 0;
    }

    return 0; // default to isolated
}
