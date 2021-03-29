#ifndef INC_8_BIT_COMPUTER_EMULATOR_H
#define INC_8_BIT_COMPUTER_EMULATOR_H

#include <memory>

#include "ArithmeticLogicUnit.h"
#include "Bus.h"
#include "Clock.h"
#include "InstructionRegister.h"
#include "MemoryAddressRegister.h"
#include "OutputRegister.h"
#include "ProgramCounter.h"
#include "RandomAccessMemory.h"
#include "Register.h"

class Emulator: public ClockListener {

public:
    Emulator();
    ~Emulator();

    void run();

private:
    std::shared_ptr<Clock> clock;
    std::shared_ptr<Bus> bus;
    std::shared_ptr<Register> aRegister;
    std::shared_ptr<Register> bRegister;
    std::unique_ptr<ArithmeticLogicUnit> alu;
    std::unique_ptr<MemoryAddressRegister> mar;
    std::unique_ptr<ProgramCounter> pc;
    std::shared_ptr<RandomAccessMemory> ram;
    std::unique_ptr<InstructionRegister> instructionRegister;
    std::unique_ptr<OutputRegister> out;

    void printValues();
    void reset();
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_H
