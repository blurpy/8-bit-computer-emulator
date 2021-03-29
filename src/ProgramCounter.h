#ifndef INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H

#include <memory>

#include "Bus.h"

class ProgramCounter {

public:
    ProgramCounter(std::shared_ptr<Bus> bus);
    ~ProgramCounter();

    void increment();
    void readFromBus();
    void writeToBus();
    void print();
    void reset();

private:
    std::shared_ptr<Bus> bus;
    uint8_t value;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
