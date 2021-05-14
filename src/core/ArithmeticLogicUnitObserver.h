#ifndef INC_8_BIT_COMPUTER_EMULATOR_ARITHMETICLOGICUNITOBSERVER_H
#define INC_8_BIT_COMPUTER_EMULATOR_ARITHMETICLOGICUNITOBSERVER_H

#include <cstdint>

namespace Core {

    /**
     * Interface for external observation of the arithmetic logic unit of the computer.
     */
    class ArithmeticLogicUnitObserver {

    public:
        /** The result of a calculation is updated, including the bits. */
        virtual void resultUpdated(uint8_t newValue, bool newCarryBit, bool newZeroBit) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_ARITHMETICLOGICUNITOBSERVER_H
