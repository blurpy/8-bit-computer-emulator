#ifndef INC_8_BIT_COMPUTER_EMULATOR_STEPCOUNTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_STEPCOUNTER_H

#include <memory>

#include "ClockListener.h"
#include "StepListener.h"
#include "ValueObserver.h"

namespace Core {

    /**
     * 3-bit counter (0->7) that keeps track of which of the 5 steps of an instruction to execute.
     * These steps are also called T-states, or timing states.
     *
     * The counter increments on the falling edge of the clock and then notifies listeners of the current step.
     */
    class StepCounter: public ClockListener {

    public:
        explicit StepCounter(const std::shared_ptr<StepListener> &stepListener);
        ~StepCounter();

        /** Reset the counter to 0. */
        void reset();

        /** Print the current counter value to standard out. */
        void print() const;

        /** Set an optional external observer of this step counter. */
        void setObserver(const std::shared_ptr<ValueObserver> &newObserver);

    private:
        uint8_t counter;
        std::shared_ptr<StepListener> stepListener;
        std::shared_ptr<ValueObserver> observer;

        void increment();
        void notifyObserver() const;
        void notifyListener() const;

        void clockTicked() override {}; // Not implemented
        void invertedClockTicked() override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_STEPCOUNTER_H
