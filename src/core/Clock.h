#ifndef INC_8_BIT_COMPUTER_CLOCK_H
#define INC_8_BIT_COMPUTER_CLOCK_H

#include <thread>
#include <vector>

#include "ClockListener.h"
#include "ClockObserver.h"
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
         * Start the clock asynchronously. Will continue until stop() or halt().
         * Note: you need to either call join() or detach() right afterwards otherwise you may experience
         * crashes or deadlocks.
         */
        void start();

        /** Stops a running asynchronous clock. */
        void stop();

        /** Halt the clock when a program is finished running, until reset(). */
        virtual void halt();

        /** Wait for an asynchronous clock while it's running. */
        void join();

        /** Ignore an asynchronous clock while it's running. */
        void detach();

        /** Run one clock cycle synchronously and then stop. */
        void singleStep();

        /** Whether the clock is currently running. */
        [[nodiscard]] bool isRunning() const;

        /** Reset the halted status of the clock to allow it to restart. */
        void reset();

        /** Set the speed to run the clock, in hertz. Must be at least 0.1. */
        void setFrequency(double newHz);

        /** Increase frequency by a step factor. */
        void increaseFrequency();

        /** Decrease frequency by a step factor, but not below 0.1. */
        void decreaseFrequency();

        /** Add a listener for clock events. */
        void addListener(const std::shared_ptr<ClockListener> &listener);

        /** Clear all the listeners. */
        void clearListeners();

        /** Set an optional external observer of this clock. */
        void setObserver(const std::shared_ptr<ClockObserver> &newObserver);

    private:
        std::shared_ptr<TimeSource> timeSource;
        double frequency;
        double hz;
        double counter;
        bool running;
        bool halted;
        bool rising;
        bool singleStepping;
        int remainingTicks;
        std::thread clockThread;
        std::vector<std::shared_ptr<ClockListener>> listeners;
        std::shared_ptr<ClockObserver> observer;

        void mainLoop();
        bool tick();
        void notifyTick() const;
        void notifyInvertedTick() const;
        void notifyFrequencyChanged() const;
    };
}

#endif //INC_8_BIT_COMPUTER_CLOCK_H
