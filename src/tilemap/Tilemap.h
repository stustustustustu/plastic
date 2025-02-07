#ifndef TILEMAP_H
#define TILEMAP_H

#include "../config.h"
#include "../utils/render/Renderer.h"
#include "../utils/render/sprites/Texture.h"
#include "Tile.h"

class Tilemap {
    public:
        int width, height;

    private:
        float TILE_SIZE;

        std::vector<std::vector<Tile>> tiles;

    public:
        Tilemap(int width, int height) : width(width), height(height), tiles(height, std::vector<Tile>(width)) {}

        Tile getTile(glm::vec2 pos) const;
        void setTile(glm::vec2 pos, const Tile& tile);

        float calculateAvgNoise(glm::vec2 startPos, int quadSize, int seed);
        int getBitmask(glm::vec2 pos, TileType type) const;
        int getVariant(int bitmask, TileType type);
};

#endif //TILEMAP_H
