#ifndef INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"
#include "ValueObserver.h"

namespace Core {

    /**
     * The register connected to the four 7-segment LEDs.
     *
     * Can read an 8-bit value from the bus and display it. And the display is currently the terminal.
     *
     * The output in the real hardware has a signed mode, allowing it to display values from -128 to 127.
     * This is the same two's compliment representation of numbers as used in the ALU for subtraction.
     * The first bit is the sign, so counting from 0 to 255 in this mode would go like this:
     *
     *      0 (0000 0000)
     *      1 (0000 0001) (skip...)
     *    126 (0111 1110)
     *    127 (0111 1111)
     *   -128 (1000 0000)
     *   -127 (1000 0001) (skip...)
     *     -1 (1111 1111)
     *
     * Signed mode is not used in any of the instructions, and therefore not implemented here. Perhaps in the future.
     */
    class OutputRegister: public ClockListener {

    public:
        explicit OutputRegister(const std::shared_ptr<Bus> &bus);
        ~OutputRegister();

        /** Print current value to standard out. */
        void print() const;

        /** Reset the register value to 0. */
        void reset();

        /** Take value from the bus on next clock tick. */
        virtual void in();

        /** Set an optional external observer of this register. */
        void setObserver(const std::shared_ptr<ValueObserver> &newObserver);

    private:
        std::shared_ptr<Bus> bus;
        std::shared_ptr<ValueObserver> observer;
        uint8_t value;
        bool readOnClock;

        void readFromBus();
        void notifyObserver() const;

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
