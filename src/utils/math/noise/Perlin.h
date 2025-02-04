#ifndef PERLIN_H
#define PERLIN_H

#include <../src/config.h>

class Perlin {
    private:
        static const int GRID_SIZE = 256;
        std::vector<int> permutation;

        float grad(int hash, float x, float y) const;
        float smoothstep(float t) const;
        float lerp(float a, float b, float t) const;

    public:
        Perlin(int seed = 0);
        void initPermutationTable(int seed);
        float getNoise(float x, float y, float frequency) const;
};



#endif //PERLIN_H
