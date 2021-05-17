#ifndef INC_8_BIT_COMPUTER_UTILS_H
#define INC_8_BIT_COMPUTER_UTILS_H

#include <bitset>

// Pattern for printf to display 3 bits in binary
#define BIT_3_PATTERN "%c%c%c"
#define BIT_3_TO_BINARY(byte) \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

// Pattern for printf to display 4 bits in binary
#define BIT_4_PATTERN "%c%c%c%c"
#define BIT_4_TO_BINARY(byte) \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

// Pattern for printf to display a byte in binary
#define BYTE_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

namespace Core {

    /**
     * Just misc utilities and constants.
     */
    class Utils {

    public:
        /** Enable debug logs? 0 = none, 1 = important, 2 = all */
        static const int DEBUG = 0;

        /** 15, or 1111 in binary, is the max value that 4 bits can represent. */
        static const int FOUR_BITS_MAX = 15;

        /** Display the most important debug logs. */
        static bool debugL1();

        /** Display all debug logs. */
        static bool debugL2();

        /** Returns the value as a 4-bit bitset. */
        static std::bitset<4> to4bits(uint8_t value);

        /** Checks if the first string starts with the value of the second string. Comes in C++20 */
        static bool startsWith(const std::string &stringToCheck, const std::string &valueToLookFor);

        /** Checks if x is less than y, handling floating point rounding errors. */
        static bool isLessThan(double x, double y);

        /** Checks if x is more or less equal to y, handling floating point rounding errors. */
        static bool equals(double x, double y);
    };
}

#endif //INC_8_BIT_COMPUTER_UTILS_H
