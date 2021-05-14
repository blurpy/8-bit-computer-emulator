#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"
#include "ValueObserver.h"

namespace Core {

    /**
     * 8-bit register that contains the currently executing instruction.
     *
     * Instructions are made up of the opcode and the operand, both 4 bits each.
     * The opcode is retrieved by the instruction decoder directly so it knows which instruction
     * to execute, while the operand can be put out on the bus for transfer to other parts of the
     * computer if the instruction requires it.
     */
    class InstructionRegister: public ClockListener {

    public:
        explicit InstructionRegister(const std::shared_ptr<Bus> &bus);
        ~InstructionRegister();

        /** Print current value to standard out. */
        void print() const;

        /** Reset the register value to 0. */
        void reset();

        /** Take the value from the bus on next clock tick. */
        virtual void in();

        /** Output the 4-bit operand from the instruction to the bus. */
        virtual void out();

        /** Get the 4-bit opcode from the instruction.  */
        [[nodiscard]] virtual uint8_t getOpcode() const;

        /** Set an optional external observer of this register. */
        void setObserver(const std::shared_ptr<ValueObserver> &newObserver);

    private:
        std::shared_ptr<Bus> bus;
        std::shared_ptr<ValueObserver> observer;
        uint8_t value;
        bool readOnClock;

        void readFromBus();
        void writeToBus();
        void notifyObserver() const;

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
