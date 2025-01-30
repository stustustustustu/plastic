#include "TimeUtils.h"

inline std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();

float getDeltaTime() {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    return elapsedTime.count();
}