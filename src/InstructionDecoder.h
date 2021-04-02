#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODER_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODER_H

#include "ArithmeticLogicUnit.h"
#include "Clock.h"
#include "FlagsRegister.h"
#include "InstructionRegister.h"
#include "MemoryAddressRegister.h"
#include "OutputRegister.h"
#include "ProgramCounter.h"
#include "RandomAccessMemory.h"
#include "Register.h"
#include "StepListener.h"

class InstructionDecoder: public StepListener {

public:
    InstructionDecoder(std::shared_ptr<MemoryAddressRegister> memoryAddressRegister,
                       std::shared_ptr<ProgramCounter> programCounter,
                       std::shared_ptr<RandomAccessMemory> randomAccessMemory,
                       std::shared_ptr<InstructionRegister> instructionRegister,
                       std::shared_ptr<Register> aRegister,
                       std::shared_ptr<Register> bRegister,
                       std::shared_ptr<ArithmeticLogicUnit> arithmeticLogicUnit,
                       std::shared_ptr<OutputRegister> outputRegister,
                       std::shared_ptr<FlagsRegister> flagsRegister,
                       std::shared_ptr<Clock> clock);
    ~InstructionDecoder();

private:
    std::shared_ptr<MemoryAddressRegister> memoryAddressRegister;
    std::shared_ptr<ProgramCounter> programCounter;
    std::shared_ptr<RandomAccessMemory> randomAccessMemory;
    std::shared_ptr<InstructionRegister> instructionRegister;
    std::shared_ptr<Register> aRegister;
    std::shared_ptr<Register> bRegister;
    std::shared_ptr<ArithmeticLogicUnit> arithmeticLogicUnit;
    std::shared_ptr<OutputRegister> outputRegister;
    std::shared_ptr<FlagsRegister> flagsRegister;
    std::shared_ptr<Clock> clock;

    void handleStep0() const;
    void handleStep1() const;
    void handleStep2() const;
    void handleStep3() const;
    void handleStep4() const;

    void stepReady(uint8_t step) override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODER_H
