#include "common/UniqueId.h"

#include <random>
#include <chrono>

std::string mui::createUniqueId() {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<unsigned int> distribution(0, std::numeric_limits<unsigned int>::max());

    auto now = std::chrono::high_resolution_clock::now();
    auto tse = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());

    return std::to_string(tse.count()) + std::to_string(distribution(generator));
}   