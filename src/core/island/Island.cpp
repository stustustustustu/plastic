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

    // Check for straight tiles
    if ((bitmask & 0b00000010) == 0b00000010 && (bitmask & 0b11101101) == 0b00000000) {
        return Tile::TOP; // Water only on top
    } else if ((bitmask & 0b00001000) == 0b00001000 && (bitmask & 0b11110111) == 0b00000000) {
        return Tile::RIGHT; // Water only on right
    } else if ((bitmask & 0b00100000) == 0b00100000 && (bitmask & 0b11011111) == 0b00000000) {
        return Tile::BOTTOM; // Water only on bottom
    } else if ((bitmask & 0b10000000) == 0b10000000 && (bitmask & 0b01111111) == 0b00000000) {
        return Tile::LEFT; // Water only on left
    }

    // Check for corners
    if ((bitmask & 0b00001110) == 0b00001110) {
        return Tile::TOP_RIGHT; // Water on top and right
    } else if ((bitmask & 0b10000010) == 0b10000010) {
        return Tile::TOP_LEFT; // Water on top and left
    } else if ((bitmask & 0b00111000) == 0b00111000) {
        return Tile::BOTTOM_RIGHT; // Water on bottom and right
    } else if ((bitmask & 0b11100000) == 0b11100000) {
        return Tile::BOTTOM_LEFT; // Water on bottom and left
    }

    // Default to fully enclosed land tile
    return Tile::CENTER;
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