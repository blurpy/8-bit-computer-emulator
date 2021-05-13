#ifndef INC_8_BIT_COMPUTER_CLOCK_H
#define INC_8_BIT_COMPUTER_CLOCK_H

#include <thread>
#include <vector>

#include "ClockListener.h"
#include "TimeSource.h"

namespace Core {

    /**
     * The clock of the computer. Used for triggering and synchronizing operations.
     *
     * The clock is a square wave at 50% duty cycle, which means it's on 50% of the time and
     * off 50% of the time. One clock cycle consists of the square wave turning on (rising edge),
     * staying there for a bit, then turning off (falling edge), and staying there for a bit.
     *
     * Listeners are notified of both those edges, as a clock tick, and an inverted clock tick.
     *
     * Remember to set the frequency before starting the clock.
     */
    class Clock {

    public:
        explicit Clock(const std::shared_ptr<TimeSource> &timeSource);
        ~Clock();

        /**
         * Start the clock asynchronously.
         * Will continue until stopped programmatically or through the HLT instruction.
         */
        void start();

        /** Stops a running asynchronous clock. */
        virtual void stop();

        /** Wait for an asynchronous clock while it's running. */
        void join();

        /** Run one clock cycle synchronously and then stop. */
        void singleStep();

        /** Set the speed to run the clock, in hertz. Must be at least 0.1. */
        void setFrequency(double hz);

        /** Add a listener for clock events. */
        void addListener(const std::shared_ptr<ClockListener> &listener);

        /** Clear all the listeners. */
        void clearListeners();

    private:
        std::shared_ptr<TimeSource> timeSource;
        double frequency;
        double counter;
        bool running;
        bool rising;
        bool singleStepping;
        int remainingTicks;
        std::thread clockThread;
        std::vector<std::shared_ptr<ClockListener>> listeners;

        void mainLoop();
        bool tick();
    };
}

#endif //INC_8_BIT_COMPUTER_CLOCK_H
