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

        /** Initialize the emulator with the program from the specified file. */
        void load(const std::string &newFileName);

        /** Resets state of the computer to the state where it was after load() and before run(). */
        void reload();

        /** Start running the loaded program. Asynchronous. */
        void startAsynchronous();

        /** Start running the loaded program. Synchronous. */
        void startSynchronous();

        /** Whether the emulator is currently running a program. */
        bool isRunning();

        /** Run one microinstruction synchronously and then stop. */
        void singleStep();

        /** Ask the emulator to stop running the current program. Asynchronous. */
        void stop();

        /** Set the speed to run the clock, in hertz. Must be set before running a program, and at least 0.1. */
        void setFrequency(double hz);

        /** Increase the frequency of the clock by a step factor. */
        void increaseFrequency();

        /** Decrease the frequency of the clock by a step factor, but not below 0.1. */
        void decreaseFrequency();

        /** Set an optional external observer of the clock. */
        void setClockObserver(const std::shared_ptr<ClockObserver> &observer);

        /** Set an optional external observer of the bus. */
        void setBusObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the A register. */
        void setARegisterObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the B register. */
        void setBRegisterObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the arithmetic logic unit. */
        void setArithmeticLogicUnitObserver(const std::shared_ptr<ArithmeticLogicUnitObserver> &observer);

        /** Set an optional external observer of the memory address register. */
        void setMemoryAddressRegisterObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the program counter. */
        void setProgramCounterObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the random access memory. */
        void setRandomAccessMemoryObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the instruction register. */
        void setInstructionRegisterObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the output register. */
        void setOutputRegisterObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the step counter. */
        void setStepCounterObserver(const std::shared_ptr<ValueObserver> &observer);

        /** Set an optional external observer of the instruction decoder. */
        void setInstructionDecoderObserver(const std::shared_ptr<InstructionDecoderObserver> &observer);

        /** Set an optional external observer of the flags register. */
        void setFlagsRegisterObserver(const std::shared_ptr<FlagsRegisterObserver> &observer);

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
        std::string fileName;

        void printValues();
        void reset();
        void initializeProgram();
        [[nodiscard]] bool programMemory();
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_H
