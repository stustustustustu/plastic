#include "Island.h"

void Island::generate(unsigned int seed) {
    std::cout << "Generating an island with seed: " << seed << std::endl;
    std::lock_guard<std::mutex> lock(mx);

    glm::vec2 centerPos = glm::vec2(tilemap.width / 2.0f, tilemap.height / 2.0f);

    for (int y = 0; y < tilemap.height; ++y) {
        for (int x = 0; x < tilemap.width; ++x) {
            float distanceFromCenter = std::sqrt((x - centerPos.x) * (x - centerPos.x) + (y - centerPos.y) * (y - centerPos.y));

            float noiseValue = stb_perlin_noise3(
                (x + seed) * 0.05f,
                (y + seed) * 0.05f,
                0.0f,
                1.0f, 1.0f, 1.0f
            );

            noiseValue = (noiseValue + 1.0f) / 2.0f;

            float falloff = 1.0f - std::pow(distanceFromCenter / radius, 2);
            falloff = std::max(0.0f, falloff);

            float value = noiseValue * falloff;

            Tile tile;
            if (value > 0.2f) {
                tile.type = SAND;
                tile.collidable = false;
            } else {
                tile.type = WATER;
                tile.collidable = true;
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
                    std::cout << "sand variant: " << tile.variant << " at coords: " << x << ", " <<  y << std::endl;
                }

                tilemap.setTile(glm::vec2(x, y), tilemap.getTile(glm::vec2(x, y)));
            }
        }
    }
}

void Island::render(Renderer* renderer, Texture* texture) {
    for (int y = 0; y < tilemap.height; ++y) {
        for (int x = 0; x < tilemap.width; ++x) {
            renderer -> DrawSpriteSheet(*texture, glm::vec2(x * 32.0f, y * 32.0f), tilemap.getTile(glm::vec2(x, y)).variant, 16, 16);
        }
    }
}