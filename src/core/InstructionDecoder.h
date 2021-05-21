#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODER_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODER_H

#include "ArithmeticLogicUnit.h"
#include "Clock.h"
#include "FlagsRegister.h"
#include "GenericRegister.h"
#include "InstructionDecoderObserver.h"
#include "InstructionRegister.h"
#include "MemoryAddressRegister.h"
#include "OutputRegister.h"
#include "ProgramCounter.h"
#include "RandomAccessMemory.h"
#include "StepListener.h"

namespace Core {

    /**
     * The instruction decoder is responsible for orchestrating the control lines of the computer
     * to make it do something useful.
     *
     * The control lines are the switches to the supported operations of the different parts,
     * like RAM In and Instruction Register Out. Typically an operation either reads (in) from the bus
     * or writes (out) to the bus.
     *
     * Every instruction takes 5 steps (microinstructions) to complete, called the instruction cycle,
     * split into 2 phases.
     *
     * The first phase is the fetch phase. It consists of 2 steps that are the same every time:
     *   step 0: Put the current value of the program counter into the memory address register.
     *   step 1: Put the value of the RAM at the current address into the instruction register and
     *           increment the program counter.
     *
     * At this point the execution phase begins, and the last 3 steps will differ for each instruction.
     * An example could be to load some data from a specified location in RAM into the B-register, and
     * store the sum of the A-register and B-register into the A-register. This is the ADD instruction.
     *
     * To make this work, the instruction decoder is notified by the step counter of which step to execute
     * on the falling edge of the clock cycle. It will then prepare the control lines for a particular
     * step of a particular instruction, and that combination is called a control word.
     * Output operations are executed right away, while input operations are executed on the
     * next rising edge of the clock cycle. Although that is the responsibility of the part itself and
     * not the instruction decoder.
     *
     * Some instructions also use flags to make decisions.
     */
    class InstructionDecoder: public StepListener {

    public:
        InstructionDecoder(const std::shared_ptr<Bus> &bus,
                           const std::shared_ptr<MemoryAddressRegister> &memoryAddressRegister,
                           const std::shared_ptr<ProgramCounter> &programCounter,
                           const std::shared_ptr<RandomAccessMemory> &randomAccessMemory,
                           const std::shared_ptr<InstructionRegister> &instructionRegister,
                           const std::shared_ptr<GenericRegister> &aRegister,
                           const std::shared_ptr<GenericRegister> &bRegister,
                           const std::shared_ptr<ArithmeticLogicUnit> &arithmeticLogicUnit,
                           const std::shared_ptr<OutputRegister> &outputRegister,
                           const std::shared_ptr<FlagsRegister> &flagsRegister,
                           const std::shared_ptr<Clock> &clock);
        ~InstructionDecoder();

        /** Set an optional external observer of this instruction decoder. */
        void setObserver(const std::shared_ptr<InstructionDecoderObserver> &newObserver);

    private:
        std::shared_ptr<Bus> bus;
        std::shared_ptr<MemoryAddressRegister> memoryAddressRegister;
        std::shared_ptr<ProgramCounter> programCounter;
        std::shared_ptr<RandomAccessMemory> randomAccessMemory;
        std::shared_ptr<InstructionRegister> instructionRegister;
        std::shared_ptr<GenericRegister> aRegister;
        std::shared_ptr<GenericRegister> bRegister;
        std::shared_ptr<ArithmeticLogicUnit> arithmeticLogicUnit;
        std::shared_ptr<OutputRegister> outputRegister;
        std::shared_ptr<FlagsRegister> flagsRegister;
        std::shared_ptr<Clock> clock;
        std::shared_ptr<InstructionDecoderObserver> observer;

        void handleStep0() const;
        void handleStep1() const;
        void handleStep2() const;
        void handleStep3() const;
        void handleStep4() const;

        void notifyObserver(const std::vector<ControlLine> &lines = {}) const;

        void stepReady(uint8_t step) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODER_H
