#include "timing.h"

#include <chrono>

uint64_t current_time_millis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}