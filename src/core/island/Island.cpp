#include "Island.h"

Island::Island(int width, int height, int seed) : WIDTH(width), HEIGHT(height) {
    if (seed == 0) {
        std::srand(time(NULL));
        seed = std::rand();
    }
    perlin.initPermutationTable(seed);
}

void Island::generate() {
    generateNoiseMap();
    applyMask();
    addSmallIslands();
    normalizeMap();
}

float Island::getNoiseValue(int x, int y) const {
    return noiseMap[x][y];
}

TileType Island::getTileType(int x, int y, float treshold) const {
    if (noiseMap[x][y] < treshold) {
        return WATER;
    }

    return SAND;
}

void Island::generateNoiseMap() {
    float frequency = 0.05f; // detail of the map
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            float nx = (float) x / WIDTH * 2.0f - 1.0f;
            float ny = (float) y / HEIGHT * 2.0f - 1.0f;
            noiseMap[x][y] = perlin.getNoise(nx, ny, frequency);
        }
    }
}

void Island::applyMask() {
    float centerX = WIDTH / 2;
    float centerY = HEIGHT / 2;
    float radius = std::min(centerX, centerY) * 0.35f;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            float distance = std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
            if (distance > radius) {
                noiseMap[x][y] -= 0.3f;
            }
        }
    }
}

void Island::addSmallIslands() {
    std::srand(time(NULL));
    float radius = std::min(WIDTH, HEIGHT) * 0.05f;
    int num = std::rand() % 3;

    for (int i = 0; i < num; ++i) {
        float islandX = rand() % WIDTH;
        float islandY = rand() % HEIGHT;

        for (int x = 0; x < WIDTH; ++x) {
            for (int y = 0; y < HEIGHT; ++y) {
                float distance = std::sqrt((x - islandX) * (x - islandX) + (y - islandY) * (y - islandY));
                if (distance <= radius) {
                    noiseMap[x][y] += 0.5f;
                }
            }
        }
    }
}

void Island::normalizeMap() {
    float min = FLT_MAX, max = -FLT_MAX;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            if (noiseMap[x][y] < min) min = noiseMap[x][y];
            if (noiseMap[x][y] > max) max = noiseMap[x][y];
        }
    }

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            noiseMap[x][y] = (noiseMap[x][y] - min) / (max - min);
        }
    }
}

void Island::renderIsland(Renderer *renderer, Texture *texture, float tileSize) {

}

void Island::printNoiseMap() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            TileType type = getTileType(x, y);

            int index = 0;
            switch (type) {
                case WATER:
                    index = 7;
                    std::cout << "w ";
                    break;
                case SAND:
                    std::cout << "s ";
                    index = 3;
                    break;
            }
        }
        std::cout << std::endl;
    }
}