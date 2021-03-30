#ifndef INC_8_BIT_COMPUTER_EMULATOR_H
#define INC_8_BIT_COMPUTER_EMULATOR_H

#include <memory>

#include "ArithmeticLogicUnit.h"
#include "Bus.h"
#include "Clock.h"
#include "InstructionRegister.h"
#include "MemoryAddressRegister.h"
#include "Microcode.h"
#include "OutputRegister.h"
#include "ProgramCounter.h"
#include "RandomAccessMemory.h"
#include "Register.h"
#include "StepCounter.h"

class Emulator {

public:
    Emulator();
    ~Emulator();

    void run(const std::string &fileName);

private:
    std::shared_ptr<Clock> clock;
    std::shared_ptr<Bus> bus;
    std::shared_ptr<Register> aRegister;
    std::shared_ptr<Register> bRegister;
    std::shared_ptr<ArithmeticLogicUnit> alu;
    std::shared_ptr<MemoryAddressRegister> mar;
    std::shared_ptr<ProgramCounter> pc;
    std::shared_ptr<RandomAccessMemory> ram;
    std::shared_ptr<InstructionRegister> instructionRegister;
    std::shared_ptr<OutputRegister> out;
    std::unique_ptr<StepCounter> stepCounter;
    std::shared_ptr<Microcode> microcode;

    void printValues();
    void reset();
    void programMemory(const std::string &fileName);
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_H
