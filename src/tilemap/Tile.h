#ifndef TILE_H
#define TILE_H

enum TileType {
    WATER,
    SAND
};

class Tile {
    public:
        TileType type;
        int variant;
        bool animated;
        bool collidable;

        Tile(TileType type = WATER, int variant = 0, bool animated = false, bool collidable = false) : type(type), variant(variant), animated(animated), collidable(collidable) {}
};

#endif //TILE_H
