#ifndef INC_8_BIT_COMPUTER_UTILS_H
#define INC_8_BIT_COMPUTER_UTILS_H

#include <bitset>

// Pattern for printf to display a byte in binary
#define BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

class Utils {

public:
    static const int DEBUG = 0; // or 1 or 2

    /** Display the most important debug logs. */
    static bool debugL1();
    /** Display all debug logs. */
    static bool debugL2();
    /** Returns the value as a 4-bit bitset. */
    static std::bitset<4> to4bits(uint8_t value);
    /** Checks if the first string starts with the value of the second string. Comes in C++20 */
    static bool startsWith(const std::string &stringToCheck, const std::string &valueToLookFor);
};

#endif //INC_8_BIT_COMPUTER_UTILS_H
