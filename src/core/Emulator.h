#ifndef INC_8_BIT_COMPUTER_EMULATOR_H
#define INC_8_BIT_COMPUTER_EMULATOR_H

#include <memory>

#include "ArithmeticLogicUnit.h"
#include "Bus.h"
#include "Clock.h"
#include "FlagsRegister.h"
#include "GenericRegister.h"
#include "InstructionDecoder.h"
#include "InstructionRegister.h"
#include "MemoryAddressRegister.h"
#include "OutputRegister.h"
#include "ProgramCounter.h"
#include "RandomAccessMemory.h"
#include "StepCounter.h"

namespace Core {

    class Emulator {

    public:
        Emulator();
        ~Emulator();

        void run(const std::string &fileName);

    private:
        std::shared_ptr<Clock> clock;
        std::shared_ptr<Bus> bus;
        std::shared_ptr<GenericRegister> aRegister;
        std::shared_ptr<GenericRegister> bRegister;
        std::shared_ptr<ArithmeticLogicUnit> arithmeticLogicUnit;
        std::shared_ptr<MemoryAddressRegister> memoryAddressRegister;
        std::shared_ptr<ProgramCounter> programCounter;
        std::shared_ptr<RandomAccessMemory> randomAccessMemory;
        std::shared_ptr<InstructionRegister> instructionRegister;
        std::shared_ptr<OutputRegister> outputRegister;
        std::shared_ptr<StepCounter> stepCounter;
        std::shared_ptr<InstructionDecoder> instructionDecoder;
        std::shared_ptr<FlagsRegister> flagsRegister;

        void printValues();
        void reset();
        [[nodiscard]] bool programMemory(const std::string &fileName);
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_H
