#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"

class InstructionRegister: public ClockListener {

public:
    explicit InstructionRegister(const std::shared_ptr<Bus> &bus);
    ~InstructionRegister();

    void print() const;
    void reset();
    void in();
    void out();
    [[nodiscard]] uint8_t getOpcode() const;

private:
    std::shared_ptr<Bus> bus;
    uint8_t value;
    bool readOnClock;

    void readFromBus();
    void writeToBus();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
