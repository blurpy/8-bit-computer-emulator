#include "Utils.h"

/**
 * This method is available to avoid warnings about value always being true/false in ifs when using the constant,
 * and unreachable code inside.
 */
bool Utils::debug() {
    return DEBUG >= 2;
}

bool Utils::debugL1() {
    return DEBUG >= 1;
}

std::bitset<4> Utils::to4bits(uint8_t value) {
    return std::bitset<4>(value);
}
