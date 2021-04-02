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
    static const int DEBUG = 0;

    static bool debug();
    static bool debugL1();
    static std::bitset<4> to4bits(uint8_t value);
};

#endif //INC_8_BIT_COMPUTER_UTILS_H
