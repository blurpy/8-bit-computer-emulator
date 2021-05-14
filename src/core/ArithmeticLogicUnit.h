#ifndef INC_8_BIT_COMPUTER_ARITHMETICLOGICUNIT_H
#define INC_8_BIT_COMPUTER_ARITHMETICLOGICUNIT_H

#include <memory>

#include "ArithmeticLogicUnitObserver.h"
#include "GenericRegister.h"
#include "RegisterListener.h"

namespace Core {

    /**
     * An 8-bit ALU that can do addition and subtraction based on the values in the A- and B-registers,
     * and output the result to the bus.
     *
     * Addition is performed as A-register + B-register and stored as soon as any of the registers change value,
     * without waiting for the clock to tick.
     *
     * Subtraction can be invoked to perform a recalculation as A-register - B-register and stored,
     * also without waiting for the clock to tick. Subtraction is a one off operation and not a state change,
     * so the result will be overwritten using addition the next time the registers change value.
     *
     * Both types of calculations result in some status bits being set.
     *
     * The bits are:
     * - Carry: whether the calculation results in a number larger than 8 bit (255) and has wrapped around.
     * - Zero: whether the calculation results in 0.
     *
     * The bits change immediately after a calculation.
     */
    class ArithmeticLogicUnit: public RegisterListener {

    public:
        ArithmeticLogicUnit(const std::shared_ptr<GenericRegister> &aRegister,
                            const std::shared_ptr<GenericRegister> &bRegister,
                            const std::shared_ptr<Bus> &bus);
        ~ArithmeticLogicUnit();

        /** Print current result to standard out. */
        void print() const;

        /** Reset result to 0. */
        void reset();

        /** Output result to the bus. */
        virtual void out();

        /** Overwrite current result with a subtraction instead. See implementation for more detailed docs. */
        virtual void subtract();

        /** Is the carry bit set. */
        [[nodiscard]] virtual bool isCarry() const;

        /** Is the zero bit set. */
        [[nodiscard]] virtual bool isZero() const;

        /** Set an optional external observer of this arithmetic logic unit. */
        void setObserver(const std::shared_ptr<ArithmeticLogicUnitObserver> &newObserver);

    private:
        std::shared_ptr<GenericRegister> aRegister;
        std::shared_ptr<GenericRegister> bRegister;
        std::shared_ptr<Bus> bus;
        std::shared_ptr<ArithmeticLogicUnitObserver> observer;
        uint8_t value;
        bool carry;
        bool zero;

        void writeToBus();
        void add();
        void notifyObserver() const;

        void registerValueChanged(uint8_t newValue) override;
    };
}

#endif //INC_8_BIT_COMPUTER_ARITHMETICLOGICUNIT_H
