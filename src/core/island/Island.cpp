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
            float value = simplexNoise(x / 7.5f, y / 7.5f);
            float falloff = gradientMask(x, y);

            noiseMap[y][x] = value * falloff;
            values.push_back(noiseMap[y][x]);
        }
    }

    std::sort(values.begin(), values.end());
    float treshold = values[static_cast<int>(values.size()) * 0.75f];

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

            game -> renderer -> DrawSpriteSheet(texture, glm::vec2(x * TILE_SIZE, y * TILE_SIZE), variant, 64, 64, glm::vec2(TILE_SIZE, TILE_SIZE));
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
        tileMap[y][x] = 1; // water
        tileVariants[y][x] = -1; // none
    } else {
        tileMap[y][x] = 1; // sand
        tileVariants[y][x] = getTileVariant(x, y); // get variant
    }

}

int Island::getTileVariant(int x, int y) {
    int bitmask = 0;

    if ((y > 0) && tileMap[y - 1][x] == 1)              bitmask |= 1; // top
    if ((y < MAP_HEIGHT - 1) && tileMap[y + 1][x] == 1) bitmask |= 2; // bottom
    if ((x > 0) && tileMap[y][x - 1] == 1)              bitmask |= 4; // left
    if ((x < MAP_WIDTH - 1) && tileMap[y][x + 1] == 1)  bitmask |= 8; // right

    static const std::map<int, int> sandVariants = {
        {0b0000, 0}, // isolated
        {0b0001, 1}, // top
        {0b0010, 2}, // bottom
        {0b0100, 3}, // left
        {0b1000, 4}, // right
        {0b0101, 5}, // vertical
        {0b0011, 6}, // horizontal
        {0b1111, 7}  // surrounded
    };

    auto it = sandVariants.find(bitmask);
    return it != sandVariants.end() ? it -> second : 0;
}

void Island::saveTileData(int x, int y, int variant) {
    tileVariants[y][x] = variant;
}
