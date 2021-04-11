#ifndef INC_8_BIT_COMPUTER_EMULATOR_REGISTERLISTENER_H
#define INC_8_BIT_COMPUTER_EMULATOR_REGISTERLISTENER_H

#include <cstdint>

namespace Core {

    /**
     * Interface for use by a register to notify a listener (implementing this interface)
     * of a change in the register value.
     */
    class RegisterListener {

    public:
        /** The register has change its value to newValue. */
        virtual void registerValueChanged(uint8_t newValue) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_REGISTERLISTENER_H
