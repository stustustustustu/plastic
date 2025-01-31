#pragma once

#include "../src/config.h"

inline int getRandomInt(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        std::srand(time(NULL));
        initialized = true;
    }
    return std::rand() % (max - min + 1) + min;
}