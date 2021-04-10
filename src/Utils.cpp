#include "Utils.h"

/*
 * These debug methods are available to avoid warnings about value always being true/false in ifs when using the
 * constant, and unreachable code inside.
 */

bool Utils::debugL1() {
    return DEBUG >= 1;
}

bool Utils::debugL2() {
    return DEBUG >= 2;
}

std::bitset<4> Utils::to4bits(uint8_t value) {
    return std::bitset<4>(value);
}

bool Utils::startsWith(const std::string &stringToCheck, const std::string &valueToLookFor) {
    if (valueToLookFor.empty()) {
        return false;
    }

    return stringToCheck.compare(0, valueToLookFor.length(), valueToLookFor) == 0;
}
