#ifndef INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
#define INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H

#include <memory>

#include "Bus.h"

class RandomAccessMemory {

public:
    RandomAccessMemory(std::shared_ptr<Bus> bus);
    ~RandomAccessMemory();

    void readFromBus();
    void writeToBus();
    void print();
    void reset();
    void setAddress(uint8_t newAddress);
    void program(uint8_t newValue);

private:
    std::shared_ptr<Bus> bus;
    uint8_t memory[16]{};
    uint8_t address;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
