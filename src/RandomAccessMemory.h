#ifndef INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
#define INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H

#include <bitset>
#include <memory>

#include "Bus.h"
#include "ClockListener.h"

class RandomAccessMemory: public ClockListener {

public:
    explicit RandomAccessMemory(std::shared_ptr<Bus> bus);
    ~RandomAccessMemory();

    void print();
    void reset();
    void setAddress(uint8_t newAddress);
    void program(std::bitset<4> opcode, std::bitset<4> operand);
    void in();
    void out();

private:
    std::shared_ptr<Bus> bus;
    uint8_t memory[16]{};
    uint8_t address;
    bool readOnClock;

    void readFromBus();
    void writeToBus();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
