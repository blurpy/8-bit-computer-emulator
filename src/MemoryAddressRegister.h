#ifndef INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H

#include <memory>

#include "Bus.h"
#include "RandomAccessMemory.h"

class MemoryAddressRegister {

public:
    MemoryAddressRegister(std::shared_ptr<RandomAccessMemory> ram, std::shared_ptr<Bus> bus);
    ~MemoryAddressRegister();

    void readFromBus();
    void print();
    void reset();
    void program(uint8_t newValue);
    void program(std::bitset<4> address);

private:
    std::shared_ptr<RandomAccessMemory> ram;
    std::shared_ptr<Bus> bus;
    uint8_t value;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H
