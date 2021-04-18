#ifndef INC_8_BIT_COMPUTER_CLOCKLISTENER_H
#define INC_8_BIT_COMPUTER_CLOCKLISTENER_H

namespace Core {

    /**
     * Interface to be implemented by those who want to be notified when the clock ticks.
     */
    class ClockListener {

    public:
        /** The rising edge of the clock is triggered. */
        virtual void clockTicked() = 0;

        /** The falling edge of the clock is triggered. */
        virtual void invertedClockTicked() = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_CLOCKLISTENER_H
