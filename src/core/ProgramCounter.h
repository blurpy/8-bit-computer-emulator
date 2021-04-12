#ifndef INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"

namespace Core {

    /**
     * 4-bit counter (0->15) that keeps track of the memory location of the next instruction to execute.
     *
     * Normal operation is to output the current value to the bus during the fetch cycle, and increment
     * by 1 afterwards. It also supports jumping to a memory location read from the bus.
     */
    class ProgramCounter: public ClockListener {

    public:
        explicit ProgramCounter(const std::shared_ptr<Bus> &bus);
        ~ProgramCounter();

        /** Print current value to standard out. */
        void print() const;
        /** Reset counter to 0. */
        void reset();
        /** Output counter value to the bus. */
        virtual void out();
        /** Increment counter value by 1 on next clock tick. */
        virtual void enable();
        /** Use value from bus as new counter value on next clock tick. */
        void jump();

    private:
        std::shared_ptr<Bus> bus;
        uint8_t value;
        bool incrementOnClock;
        bool readOnClock;

        void increment();
        void readFromBus();
        void writeToBus();

        void clockTicked() override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
