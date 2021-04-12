#ifndef INC_8_BIT_COMPUTER_EMULATOR_STEPCOUNTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_STEPCOUNTER_H

#include <memory>

#include "ClockListener.h"
#include "StepListener.h"

namespace Core {

    class StepCounter: public ClockListener {

    public:
        explicit StepCounter(const std::shared_ptr<StepListener> &stepListener);
        ~StepCounter();

        void reset();
        void print() const;

    private:
        uint8_t counter;
        bool init;
        std::shared_ptr<StepListener> stepListener;

        void increment();

        void clockTicked() override {}; // Not implemented
        void invertedClockTicked() override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_STEPCOUNTER_H
