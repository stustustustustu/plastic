#include "Island.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

Island::Island(unsigned int seed) : MAP_WIDTH(game -> getInstance() -> getSize().at(0) / TILE_SIZE), MAP_HEIGHT(game -> getInstance() -> getSize().at(1) / TILE_SIZE) {
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoise::Simplex);

    noiseMap.resize(MAP_HEIGHT * TILE_SIZE, std::vector<float>(MAP_WIDTH * TILE_SIZE, 0.0f));
    tileMap.resize(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, 0)); // 0 water, 1 sand
    tileVariants.resize(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, 0)); // variant for sand

    generate();
}

void Island::generate() {
    std::vector<float> values;

    for (int y = 0; y < MAP_HEIGHT * TILE_SIZE; ++y) {
        for (int x = 0; x < MAP_WIDTH * TILE_SIZE; ++x) {
            float value = simplexNoise(x / 10.0f, y / 10.0f);
            float falloff = gradientMask(x, y);

            noiseMap[y][x] = value * falloff;
            values.push_back(noiseMap[y][x]);
        }
    }

    std::sort(values.begin(), values.end());
    float treshold = values[static_cast<int>(values.size()) * 0.70f];

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            classifyTile(x, y, treshold);
        }
    }

    std::cout << "Generated an island with the seed of: " << seed << ", using treshold: " << treshold << std::endl;
}

void Island::render(Texture &texture) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int variant = tileVariants[y][x];

            if (tileMap[y][x] != 0) {
                game -> renderer -> DrawSpriteSheet(texture, glm::vec2(x * TILE_SIZE, y * TILE_SIZE), variant, 32, 32, glm::vec2(TILE_SIZE, TILE_SIZE));
            }
        }
    }
}

float Island::simplexNoise(float x, float y) {
    return noise.GetNoise(x, y);
}

float Island::gradientMask(float x, float y) {
    float nx = (x - MAP_WIDTH * TILE_SIZE / 2.0f) / (MAP_WIDTH * TILE_SIZE / 2.0f);
    float ny = (y - MAP_HEIGHT * TILE_SIZE / 2.0f) / (MAP_HEIGHT * TILE_SIZE / 2.0f);

    float distance = std::sqrt(nx * nx + ny * ny);

    return std::clamp(1.0f - distance, 0.0f, 1.0f);
}

void Island::classifyTile(int x, int y, float treshold) {
    float avg = 0.0f;

    for (int dy = 0; dy < TILE_SIZE; ++dy) {
        for (int dx = 0; dx < TILE_SIZE; ++dx) {
            avg += noiseMap[y * TILE_SIZE + dy][x * TILE_SIZE + dx];
        }
    }

    avg /= std::pow(TILE_SIZE, 2);

    if (avg < treshold) {
        tileMap[y][x] = 0; // water
        tileVariants[y][x] = -1; // none
    } else {
        tileMap[y][x] = 1; // sand
        tileVariants[y][x] = getTileVariant(x, y); // get variant
    }
}

int Island::getTileVariant(int x, int y) {
    int bitmask = 1;

    return bitmask;
}

void Island::saveTileData(int x, int y, int variant) {
    tileVariants[y][x] = variant;
}
