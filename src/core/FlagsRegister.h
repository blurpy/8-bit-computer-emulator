#ifndef INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTER_H

#include "ArithmeticLogicUnit.h"
#include "ClockListener.h"

namespace Core {

    /**
     * Register to store flags.
     *
     * The flags are:
     * - Carry: whether the ALU calculation results in a number larger than 8 bit (255) and has wrapped around.
     * - Zero: whether the ALU calculation results in 0.
     */
    class FlagsRegister: public ClockListener {

    public:
        explicit FlagsRegister(const std::shared_ptr<ArithmeticLogicUnit> &arithmeticLogicUnit);
        ~FlagsRegister();

        /** Print current flag values to standard out. */
        void print() const;
        /** Reset flags to false. */
        void reset();
        /** Use carry and zero bits from ALU as new flag values on next clock tick. */
        virtual void in();
        /** Is the carry flag set. */
        [[nodiscard]] virtual bool isCarryFlag() const;
        /** Is the zero flag set. */
        [[nodiscard]] virtual bool isZeroFlag() const;

    private:
        std::shared_ptr<ArithmeticLogicUnit> arithmeticLogicUnit;
        bool readOnClock;
        bool carryFlag;
        bool zeroFlag;

        void readFromAlu();

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTER_H
