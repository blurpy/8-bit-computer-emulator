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

    /**
     * This class brings all the different components together to make the computer work.
     */
    class Emulator {

    public:
        Emulator();
        ~Emulator();

        /** Start the emulator, load the program from the specified file, and run it. */
        void run(const std::string &fileName);

        /** Set the speed to run the clock, in hertz. Must be set before running a program, and at least 0.1. */
        void setFrequency(double hz);

        /** Set an optional external observer of the output register. */
        void setOutputRegisterObserver(const std::shared_ptr<ValueObserver> &observer);

    private:
        std::shared_ptr<TimeSource> timeSource;
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
