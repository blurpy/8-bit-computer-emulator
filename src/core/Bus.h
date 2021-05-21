#ifndef INC_8_BIT_COMPUTER_BUS_H
#define INC_8_BIT_COMPUTER_BUS_H

#include <cstdint>
#include <memory>

#include "ValueObserver.h"

namespace Core {

    /**
     * 8-bit bus that can be read and written to at any point.
     */
    class Bus {

    public:
        Bus();
        ~Bus();

        /** Get current value on the bus. */
        [[nodiscard]] uint8_t read() const;

        /** Write a new value to the bus. */
        void write(uint8_t newValue);

        /** Print current value to standard out. */
        void print() const;

        /** Reset the bus to 0. */
        virtual void reset();

        /** Set an optional external observer of this bus. */
        void setObserver(const std::shared_ptr<ValueObserver> &newObserver);

    private:
        std::shared_ptr<ValueObserver> observer;
        uint8_t value;

        void notifyObserver() const;
    };
}

#endif //INC_8_BIT_COMPUTER_BUS_H
