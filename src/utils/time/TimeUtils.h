#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include "../src/config.h"

extern std::chrono::steady_clock::time_point lastUpdateTime;

float getDeltaTime();

#endif //TIMEUTILS_H
