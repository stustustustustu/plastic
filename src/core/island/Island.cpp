#include "Island.h"
#include "../src/Game.h"
#include <algorithm>
#include <iostream>
#include <cmath>

const auto game = Game::getInstance();

Island::Island(unsigned int seed) : MAP_WIDTH(game->getInstance()->getSize().at(0) / TILE_SIZE), MAP_HEIGHT(game->getInstance()->getSize().at(1) / TILE_SIZE) {
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoise::Simplex);

    noiseMap.resize(MAP_HEIGHT * TILE_SIZE, std::vector<float>(MAP_WIDTH * TILE_SIZE, 0.0f));
    tileMap.resize(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, static_cast<int>(Tile::WATER))); // Initialize all tiles as water
    tileVariants.resize(MAP_HEIGHT, std::vector<Tile>(MAP_WIDTH, Tile::WATER)); // Initialize variants

    generate();
}

void Island::generate() {
    // Step 1: Generate the initial island
    for (int y = 0; y < MAP_HEIGHT * TILE_SIZE; ++y) {
        for (int x = 0; x < MAP_WIDTH * TILE_SIZE; ++x) {
            float value = simplexNoise(x / 10.0f, y / 10.0f);
            float falloff = gradientMask(x, y);

            noiseMap[y][x] = value * falloff;
        }
    }

    // Step 2: Classify tiles as land or water
    std::vector<float> values;
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            float avg = 0.0f;

            for (int dy = 0; dy < TILE_SIZE; ++dy) {
                for (int dx = 0; dx < TILE_SIZE; ++dx) {
                    avg += noiseMap[y * TILE_SIZE + dy][x * TILE_SIZE + dx];
                }
            }

            avg /= std::pow(TILE_SIZE, 2);
            values.push_back(avg);
        }
    }

    std::sort(values.begin(), values.end());
    float threshold = values[static_cast<int>(values.size() * 0.70f)]; // 30% land

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            float avg = 0.0f;

            for (int dy = 0; dy < TILE_SIZE; ++dy) {
                for (int dx = 0; dx < TILE_SIZE; ++dx) {
                    avg += noiseMap[y * TILE_SIZE + dy][x * TILE_SIZE + dx];
                }
            }

            avg /= std::pow(TILE_SIZE, 2);

            if (avg < threshold) {
                tileMap[y][x] = static_cast<int>(Tile::WATER); // Water
            } else {
                tileMap[y][x] = static_cast<int>(Tile::CENTER); // Land (will be updated later)
            }
        }
    }

    // Step 3: Trace the island edge and place beach tiles
    traceIslandEdge();

    std::cout << "Generated an island with the seed of: " << seed << ", using threshold: " << threshold << std::endl;
}

void Island::traceIslandEdge() {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            // Skip water tiles
            if (tileMap[y][x] == static_cast<int>(Tile::WATER)) {
                continue;
            }

            // Check if this tile is on the edge of the island
            bool isEdgeTile = false;
            if (isWater(x, y - 1) || isWater(x + 1, y) || isWater(x, y + 1) || isWater(x - 1, y) ||
                isWater(x - 1, y - 1) || isWater(x + 1, y + 1) || isWater(x + 1, y - 1) || isWater(x - 1, y + 1)) {
                isEdgeTile = true;
            }

            if (isEdgeTile) {
                // Determine the correct beach tile variant
                tileMap[y][x] = static_cast<int>(getBeachTileVariant(x, y));
            } else {
                // This tile is fully enclosed by land
                tileMap[y][x] = static_cast<int>(Tile::CENTER);
            }
        }
    }
}

Tile Island::getBeachTileVariant(int x, int y) {
    // Create an 8-bit bitmask for neighboring water tiles
    int bitmask = 0;
    bitmask |= isWater(x - 1, y - 1) ? 0b00000001 : 0; // Top-left
    bitmask |= isWater(x, y - 1) ? 0b00000010 : 0;     // Top
    bitmask |= isWater(x + 1, y - 1) ? 0b00000100 : 0; // Top-right
    bitmask |= isWater(x + 1, y) ? 0b00001000 : 0;     // Right
    bitmask |= isWater(x + 1, y + 1) ? 0b00010000 : 0; // Bottom-right
    bitmask |= isWater(x, y + 1) ? 0b00100000 : 0;     // Bottom
    bitmask |= isWater(x - 1, y + 1) ? 0b01000000 : 0; // Bottom-left
    bitmask |= isWater(x - 1, y) ? 0b10000000 : 0;     // Left

    // straights
    switch (bitmask) {
        case 0b00000010:
        case 0b00000110:
        case 0b00000011:
        case 0b00000111:
            return Tile::TOP;
        case 0b00001000:
        case 0b00001100:
        case 0b00011000:
        case 0b00011100:
            return Tile::RIGHT;
        case 0b00100000:
        case 0b00110000:
        case 0b01100000:
        case 0b01110000:
            return Tile::BOTTOM;
        case 0b10000000:
        case 0b11000000:
        case 0b10000001:
        case 0b11000001:
            return Tile::LEFT;
        default: break;
    }

    // inner corners
    switch (bitmask) {


        default: break;
    }

    // outter corners
    switch (bitmask) {
        case 0b00001110: return Tile::REVERSE_TOP_RIGHT;
        case 0b10000011: return Tile::REVERSE_TOP_LEFT;
        case 0b00111000: return Tile::REVERSE_BOTTOM_RIGHT;
        case 0b11100000: return Tile::REVERSE_BOTTOM_LEFT;
        default: break;
    }

    return Tile::ISOLATED;
}

bool Island::isLand(int x, int y) const {
    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
        return tileMap[y][x] != static_cast<int>(Tile::WATER);
    }
    return false;
}

bool Island::isWater(int x, int y) const {
    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
        return tileMap[y][x] == static_cast<int>(Tile::WATER);
    }
    return true;
}

float Island::simplexNoise(float x, float y) {
    return noise.GetNoise(x, y);
}

float Island::gradientMask(float x, float y) {
    // Normalize coordinates to the range [-1, 1]
    float nx = (x - MAP_WIDTH * TILE_SIZE / 2.0f) / (MAP_WIDTH * TILE_SIZE / 2.0f);
    float ny = (y - MAP_HEIGHT * TILE_SIZE / 2.0f) / (MAP_HEIGHT * TILE_SIZE / 2.0f);

    // Calculate distance from the center
    float distance = std::sqrt(nx * nx + ny * ny);

    // Apply a circular falloff
    float falloff = 1.0f - distance; // Linear falloff
    falloff = std::clamp(falloff, 0.0f, 1.0f); // Clamp to [0, 1]

    // Apply a stronger falloff near the edges
    falloff = std::pow(falloff, 2.0f); // Quadratic falloff for a smoother transition

    return falloff;
}

void Island::render(Texture &texture) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int variant = tileMap[y][x]; // Use tileMap directly for rendering

            if (tileMap[y][x] != static_cast<int>(Tile::WATER)) {
                game->renderer->DrawSpriteSheet(texture, glm::vec2(x * TILE_SIZE, y * TILE_SIZE), variant, 32, 32, glm::vec2(TILE_SIZE, TILE_SIZE));
            }
        }
    }
}