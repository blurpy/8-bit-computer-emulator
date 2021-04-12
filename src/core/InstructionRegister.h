#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"

namespace Core {

    class InstructionRegister: public ClockListener {

    public:
        explicit InstructionRegister(const std::shared_ptr<Bus> &bus);
        ~InstructionRegister();

        void print() const;
        void reset();
        virtual void in();
        virtual void out();
        [[nodiscard]] virtual uint8_t getOpcode() const;

    private:
        std::shared_ptr<Bus> bus;
        uint8_t value;
        bool readOnClock;

        void readFromBus();
        void writeToBus();

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONREGISTER_H
