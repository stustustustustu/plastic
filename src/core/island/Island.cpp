#include "Island.h"
#include "../src/Game.h"
#include <algorithm>
#include <iostream>
#include <cmath>

const auto game = Game::getInstance();

Island::Island(unsigned int seed) : MAP_WIDTH(game->getInstance()->getSize().at(0) / TILE_SIZE), MAP_HEIGHT(game->getInstance()->getSize().at(1) / TILE_SIZE) {
    this -> seed = seed;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoise::Simplex);

    noiseMap.resize(MAP_HEIGHT * TILE_SIZE, std::vector<float>(MAP_WIDTH * TILE_SIZE, 0.0f));
    tileMap.resize(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, static_cast<int>(Tile::WATER))); // Initialize all tiles as water
    tileVariants.resize(MAP_HEIGHT, std::vector<Tile>(MAP_WIDTH, Tile::WATER)); // Initialize variants

    generate();
}

void Island::generate() {
    for (int y = 0; y < MAP_HEIGHT * TILE_SIZE; ++y) {
        for (int x = 0; x < MAP_WIDTH * TILE_SIZE; ++x) {
            float value = simplexNoise(x / 10.0f, y / 10.0f);
            float falloff = gradientMask(x, y);

            noiseMap[y][x] = value * falloff;
        }
    }

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
    float threshold = values[static_cast<int>(values.size() * 0.70f)];

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
                tileMap[y][x] = static_cast<int>(Tile::WATER);
            } else {
                tileMap[y][x] = static_cast<int>(Tile::CENTER);
            }
        }
    }

    traceIslandEdge();

    std::cout << "Generated an island with the seed of: " << seed << ", using threshold: " << threshold << std::endl;
}

void Island::traceIslandEdge() {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (tileMap[y][x] == static_cast<int>(Tile::WATER)) continue;

            bool isEdgeTile = false;
            if (isWater(x, y - 1) || isWater(x + 1, y) || isWater(x, y + 1) || isWater(x - 1, y) ||
                isWater(x - 1, y - 1) || isWater(x + 1, y + 1) || isWater(x + 1, y - 1) || isWater(x - 1, y + 1)) {
                isEdgeTile = true;
            }

            if (isEdgeTile) {
                Tile variant = getBeachTileVariant(x, y);
                if (variant == Tile::EMPTY) {
                    tileMap[y][x] = static_cast<int>(Tile::WATER);
                } else {
                    tileMap[y][x] = static_cast<int>(variant);
                }
            } else {
                tileMap[y][x] = static_cast<int>(Tile::CENTER);
            }
        }
    }
}

Tile Island::getBeachTileVariant(int x, int y) const {
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
        case 0b00000101:
            return Tile::TOP;
        case 0b00001000:
        case 0b00001100:
        case 0b00011000:
        case 0b00011100:
        case 0b00010100:
            return Tile::RIGHT;
        case 0b00100000:
        case 0b00110000:
        case 0b01100000:
        case 0b01110000:
        case 0b01010000:
            return Tile::BOTTOM;
        case 0b10000000:
        case 0b11000000:
        case 0b10000001:
        case 0b11000001:
        case 0b01000001:
            return Tile::LEFT;

        default: break;
    }

    // inner corners
    switch (bitmask) {
        case 0b00000100:
            return Tile::TOP_RIGHT;
        case 0b00000001:
            return Tile::TOP_LEFT;
        case 0b00010000:
            return Tile::BOTTOM_RIGHT;
        case 0b01000000:
            return Tile::BOTTOM_LEFT;

        default: break;
    }

    // outer corners
    switch (bitmask) {
        case 0b00001110:
        case 0b00001111:
        case 0b00011110:
        case 0b00011111:
            return Tile::REVERSE_TOP_RIGHT;
        case 0b10000011:
        case 0b10000111:
        case 0b11000011:
        case 0b11000111:
            return Tile::REVERSE_TOP_LEFT;
        case 0b00111000:
        case 0b01111000:
        case 0b00111100:
        case 0b01111100:
            return Tile::REVERSE_BOTTOM_RIGHT;
        case 0b11100000:
        case 0b11110000:
        case 0b11100001:
        case 0b11110001:
            return Tile::REVERSE_BOTTOM_LEFT;

        default: break;
    }

    // diagonals
    switch (bitmask) {
        case 0b01000100:
            return Tile::DIAGONAL_TR_BL;
        case 0b00010001:
            return Tile::DIAGONAL_TL_BR;

        default: break;
    }

    // edge cases
    switch (bitmask) {
        case 0b01001100:
        case 0b01011000:
        case 0b01010100:
        case 0b01110100:
        case 0b01011100:
        case 0b00110100:
        case 0b01100100:
            return Tile::REVERSE_BOTTOM_RIGHT;

        case 0b00110001:
        case 0b11010001:
        case 0b01110001:
        case 0b01100001:
        case 0b10010000:
        case 0b10100000:
        case 0b10110000:
            return Tile::REVERSE_BOTTOM_LEFT;

        case 0b00010111:
        case 0b00011101:
        case 0b00010011:
        case 0b00010110:
        case 0b00011001:
        case 0b00001001:
        case 0b00001011:
            return Tile::REVERSE_TOP_RIGHT;

        case 0b01010001:
        case 0b01000111:
        case 0b11000101:
        case 0b01000011:
        case 0b11000100:
            return Tile::REVERSE_TOP_LEFT;

        default: break;
    }

    return Tile::EMPTY;
}

std::pair<int, int> Island::findNearestLandTile(float screenX, float screenY) const {
    int centerX = static_cast<int>(screenX) / TILE_SIZE;
    int centerY = static_cast<int>(screenY) / TILE_SIZE;

    centerX = std::clamp(centerX, 0, MAP_WIDTH - 1);
    centerY = std::clamp(centerY, 0, MAP_HEIGHT - 1);

    if (isLand(centerX, centerY)) {
        return {centerX, centerY};
    }

    int maxRadius = std::max(MAP_WIDTH, MAP_HEIGHT);
    for (int radius = 1; radius < maxRadius; ++radius) {
        for (int dx = -radius; dx <= radius; ++dx) {
            for (int dy = -radius; dy <= radius; ++dy) {
                if (abs(dx) != radius && abs(dy) != radius) continue;

                int x = centerX + dx;
                int y = centerY + dy;
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                    if (isLand(x, y)) {
                        return {x, y};
                    }
                }
            }
        }
    }

    return {centerX, centerY};
}

float Island::distanceToNearestLand(float worldX, float worldY) const {
    float minDistance = FLT_MAX;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (isLand(x, y)) {
                float dx = (x * TILE_SIZE + TILE_SIZE/2) - worldX;
                float dy = (y * TILE_SIZE + TILE_SIZE/2) - worldY;
                float distance = std::sqrt(dx*dx + dy*dy);
                if (distance < minDistance) {
                    minDistance = distance;
                }
            }
        }
    }
    return minDistance;
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
    float nx = (x - MAP_WIDTH * TILE_SIZE / 2.0f) / (MAP_WIDTH * TILE_SIZE / 2.0f);
    float ny = (y - MAP_HEIGHT * TILE_SIZE / 2.0f) / (MAP_HEIGHT * TILE_SIZE / 2.0f);

    float distance = std::sqrt(nx * nx + ny * ny);

    float falloff = 1.0f - distance;
    falloff = std::clamp(falloff, 0.0f, 1.0f);

    falloff = std::pow(falloff, 2.0f);

    return falloff;
}

void Island::render(Texture &texture) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int variant = tileMap[y][x];

            if (tileMap[y][x] != static_cast<int>(Tile::WATER)) {
                game -> renderer -> DrawSpriteSheet(texture, glm::vec2(x * TILE_SIZE, y * TILE_SIZE), variant, 32, 32, glm::vec2(TILE_SIZE, TILE_SIZE));
            }
        }
    }
}