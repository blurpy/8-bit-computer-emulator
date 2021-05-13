#ifndef INC_8_BIT_COMPUTER_EMULATOR_VALUEOBSERVER_H
#define INC_8_BIT_COMPUTER_EMULATOR_VALUEOBSERVER_H

#include <cstdint>

namespace Core {

    /**
     * Interface for external observation of core components of the computer.
     * This is a generic observer for components with only a single observable value.
     */
    class ValueObserver {

    public:
        /** The observed value has changed to the following new value. */
        virtual void valueUpdated(uint8_t newValue) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_VALUEOBSERVER_H
