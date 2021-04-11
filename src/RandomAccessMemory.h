#ifndef INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
#define INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H

#include <bitset>
#include <memory>

#include "Bus.h"
#include "ClockListener.h"
#include "RegisterListener.h"

class RandomAccessMemory: public ClockListener, public RegisterListener {

public:
    explicit RandomAccessMemory(const std::shared_ptr<Bus> &bus);
    ~RandomAccessMemory();

    void print();
    void reset();
    void program(const std::bitset<4> &opcode, const std::bitset<4> &operand);
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
    void registerValueChanged(uint8_t newValue) override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
