#include "Perlin.h"

Perlin::Perlin(int seed) {
    if (seed == 0) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seed = std::rand();
    }
    initPermutationTable(seed);
}

float Perlin::getNoise(float x, float y, float frequency) const {
    float xf = x * frequency;
    float yf = y * frequency;

    int xi = static_cast<int>(std::floor(xf)) & (GRID_SIZE - 1); // Wrap around using bitwise AND
    int yi = static_cast<int>(std::floor(yf)) & (GRID_SIZE - 1);

    float xf_frac = xf - std::floor(xf); // Fractional part
    float yf_frac = yf - std::floor(yf);

    float u = smoothstep(xf_frac);
    float v = smoothstep(yf_frac);

    // Correctly wrap around the indices
    int aa = permutation[permutation[xi] + yi];
    int ab = permutation[permutation[xi] + ((yi + 1) & (GRID_SIZE - 1))];
    int ba = permutation[permutation[(xi + 1) & (GRID_SIZE - 1)] + yi];
    int bb = permutation[permutation[(xi + 1) & (GRID_SIZE - 1)] + ((yi + 1) & (GRID_SIZE - 1))];

    // Compute the dot products
    float x0 = grad(aa, xf_frac, yf_frac);
    float x1 = grad(ba, xf_frac - 1.0f, yf_frac);
    float y0 = lerp(x0, x1, u);

    float x2 = grad(ab, xf_frac, yf_frac - 1.0f);
    float x3 = grad(bb, xf_frac - 1.0f, yf_frac - 1.0f);
    float y1 = lerp(x2, x3, u);

    return lerp(y0, y1, v);
}

void Perlin::initPermutationTable(int seed) {
    permutation.resize(GRID_SIZE);
    for (int i = 0; i < GRID_SIZE; ++i) {
        permutation[i] = i;
    }
    std::srand(seed);
    for (int i = GRID_SIZE - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(permutation[i], permutation[j]);
    }
}

float Perlin::grad(int hash, float x, float y) const {
    int h = hash & 15; // Take the low 4 bits of the hash
    float u = h < 8 ? x : y; // If the high bit of h is 0, use x; otherwise, use y
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0.0f); // Use y if h is 0, 1, 4, or 5; otherwise, use x or 0
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // Compute the dot product
}

float Perlin::smoothstep(float t) const {
    return t * t * (3.0f - 2.0f * t);
}

float Perlin::lerp(float a, float b, float t) const {
    return a + t * (b - a);
}