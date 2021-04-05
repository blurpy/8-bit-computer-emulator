#ifndef INC_8_BIT_COMPUTER_BUS_H
#define INC_8_BIT_COMPUTER_BUS_H

#include <cstdint>

/**
 * 8-bit bus that can be read and written to at any point.
 */
class Bus {

public:
    Bus();
    ~Bus();

    /** Get current value on the bus. */
    [[nodiscard]] uint8_t read() const;
    /** Write a new value to the bus. */
    void write(uint8_t newValue);
    /** Print current value to standard out. */
    void print() const;
    /** Reset the bus to 0. */
    void reset();

private:
    uint8_t value;
};

#endif //INC_8_BIT_COMPUTER_BUS_H
