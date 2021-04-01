#ifndef INC_8_BIT_COMPUTER_EMULATOR_MICROCODE_H
#define INC_8_BIT_COMPUTER_EMULATOR_MICROCODE_H

#include "ArithmeticLogicUnit.h"
#include "Clock.h"
#include "InstructionRegister.h"
#include "MemoryAddressRegister.h"
#include "OutputRegister.h"
#include "ProgramCounter.h"
#include "RandomAccessMemory.h"
#include "Register.h"
#include "StepListener.h"

class Microcode: public StepListener {

public:
    Microcode(std::shared_ptr<MemoryAddressRegister> mar, std::shared_ptr<ProgramCounter> pc,
              std::shared_ptr<RandomAccessMemory> ram, std::shared_ptr<InstructionRegister> instructionRegister,
              std::shared_ptr<Register> aRegister, std::shared_ptr<Register> bRegister,
              std::shared_ptr<ArithmeticLogicUnit> alu, std::shared_ptr<OutputRegister> out,
              std::shared_ptr<Clock> clock);
    ~Microcode();

private:
    std::shared_ptr<MemoryAddressRegister> mar;
    std::shared_ptr<ProgramCounter> pc;
    std::shared_ptr<RandomAccessMemory> ram;
    std::shared_ptr<InstructionRegister> instructionRegister;
    std::shared_ptr<Register> aRegister;
    std::shared_ptr<Register> bRegister;
    std::shared_ptr<ArithmeticLogicUnit> alu;
    std::shared_ptr<OutputRegister> out;
    std::shared_ptr<Clock> clock;

    void handleStep0() const;
    void handleStep1() const;
    void handleStep2() const;
    void handleStep3() const;
    void handleStep4() const;

    void stepReady(uint8_t step) override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_MICROCODE_H
