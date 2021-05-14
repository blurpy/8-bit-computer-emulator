#ifndef INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H

#include <bitset>
#include <memory>

#include "Bus.h"
#include "ClockListener.h"
#include "RegisterListener.h"

namespace Core {

    /**
     * A 4-bit (0->15) register that keeps track of the active memory address.
     *
     * The RAM is notified right away when an address changes, without waiting for the clock.
     *
     * Supports manual control like the DIP-switches using the program function.
     */
    class MemoryAddressRegister: public ClockListener {

    public:
        MemoryAddressRegister(const std::shared_ptr<RegisterListener> &registerListener,
                              const std::shared_ptr<Bus> &bus);
        ~MemoryAddressRegister();

        /** Print current value to standard out. */
        void print() const;

        /** Reset the register value to 0. */
        void reset();

        /** Sets the address to use in manual mode. */
        void program(const std::bitset<4> &address);

        /** Take a 4-bit value from the bus on next clock tick. */
        virtual void in();

        /** Set an optional external observer of this register. */
        void setObserver(const std::shared_ptr<ValueObserver> &newObserver);

    private:
        std::shared_ptr<RegisterListener> registerListener;
        std::shared_ptr<Bus> bus;
        std::shared_ptr<ValueObserver> observer;
        uint8_t value;
        bool readOnClock;

        void readFromBus();
        void notifyObserver() const;
        void notifyListener() const;

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H
