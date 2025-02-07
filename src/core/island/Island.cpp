#include "Island.h"

void Island::generate(unsigned int seed) {
    std::cout << "Generating and island with seed: " << seed << std::endl;

    std::lock_guard<std::mutex> lock(mx);

    glm::vec2 centerPos = glm::vec2(tilemap.width / 2.0f, tilemap.height / 2.0f);

    for (int y = 0; y < tilemap.height; ++y) {
        for (int x = 0; x < tilemap.width; ++x) {
            float avgNoise = tilemap.calculateAvgNoise(glm::vec2(x, y), 1, seed);
            avgNoise = (avgNoise + 1.0f) / 2.0f;
            float distanceCenter = std::sqrt((x - centerPos.x) * (x - centerPos.x) + (y - centerPos.y) * (y - centerPos.y));
            float value = avgNoise * (1.0f - (distanceCenter / radius));

            Tile tile;

            if (value > 0.45f) {
                tile.type = SAND;
                tile.collidable = false;
            } else {
                tile.type = WATER;
                tile.collidable = false;
            }

            tilemap.setTile(glm::vec2(x, y), tile);
        }
    }

    applyBitmask();

    complete = true;
    cv.notify_one();
}

void Island::applyBitmask() {
    for (int y = 0; y < tilemap.height; ++y) {
        for (int x = 0; x < tilemap.width; ++x) {
            Tile tile = tilemap.getTile(glm::vec2(x, y));
            TileType type = tilemap.getTile(glm::vec2(x, y)).type;

            if (type == SAND) {
                int bitmask = tilemap.getBitmask(glm::vec2(x, y), type);
                tile.variant = tilemap.getVariant(bitmask, type);

                if (tile.variant != 0) {
                    //std::cout << "sand variant: " << tile.variant << " at coords: " << x << ", " <<  y << std::endl;
                }

                tilemap.setTile(glm::vec2(x, y), tilemap.getTile(glm::vec2(x, y)));
            }
        }
    }
}


void Island::render(Renderer* renderer, Texture* texture) {
    std::vector<glm::vec2> positions;
    std::vector<int> indices;

    for (int y = 0; y < tilemap.height; ++y) {
        for (int x = 0; x < tilemap.width; ++x) {
            Tile tile = tilemap.getTile(glm::vec2(x, y));

            positions.push_back(glm::vec2(x * 32.0f, y * 32.0f));
            indices.push_back(tile.variant);
        }
    }

    renderer -> DrawTilemap(*texture, positions, indices, 32.0f);
}