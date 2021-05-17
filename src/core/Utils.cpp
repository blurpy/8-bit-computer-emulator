#include <cmath>

#include "Utils.h"

/*
 * These debug methods are available to avoid warnings about value always being true/false in ifs when using the
 * constant, and unreachable code inside.
 */

bool Core::Utils::debugL1() {
    return DEBUG >= 1;
}

bool Core::Utils::debugL2() {
    return DEBUG >= 2;
}

std::bitset<4> Core::Utils::to4bits(const uint8_t value) {
    return std::bitset<4>(value);
}

bool Core::Utils::startsWith(const std::string &stringToCheck, const std::string &valueToLookFor) {
    if (valueToLookFor.empty()) {
        return false;
    }

    return stringToCheck.compare(0, valueToLookFor.length(), valueToLookFor) == 0;
}

bool Core::Utils::isLessThan(double x, double y) {
    if (x >= y) {
        return false;
    }

    // Feels hacky. There must be a better way...
    return std::fabs(x - y) > 0.000001; //std::numeric_limits<double>::epsilon();;
}

bool Core::Utils::equals(double x, double y) {
    if (x == y) {
        return true;
    }

    // Feels hacky. There must be a better way...
    return std::fabs(x - y) <= 0.000001; //std::numeric_limits<double>::epsilon();
}
