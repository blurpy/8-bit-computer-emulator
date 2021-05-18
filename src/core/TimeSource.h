#ifndef INC_8_BIT_COMPUTER_EMULATOR_TIMESOURCE_H
#define INC_8_BIT_COMPUTER_EMULATOR_TIMESOURCE_H

#include <chrono>

namespace Core {

    /**
     * Helper class for sleeping and keeping track of time.
     *
     * 1 second      = 1000 milliseconds
     * 1 millisecond = 1000 microseconds
     * 1 microsecond = 1000 nanoseconds
     */
    class TimeSource {

    public:
        TimeSource();
        ~TimeSource();

        /** Reset time delta to 0. */
        virtual void reset();

        /** Return amount of time in nanoseconds that has passed since last time this method or reset was called. */
        virtual double delta();

        /** Sleeps the current thread the specified number of nanoseconds. */
        virtual void sleep(long nanoseconds) const;

    private:
        std::chrono::steady_clock::time_point lastTime;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_TIMESOURCE_H
