#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H

#include <memory>

#include "Bus.h"

class InstructionRegister {

public:
    InstructionRegister(std::shared_ptr<Bus> bus);
    ~InstructionRegister();

    void readFromBus();
    void writeToBus();
    void print();
    void reset();

private:
    std::shared_ptr<Bus> bus;
    uint8_t value;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
