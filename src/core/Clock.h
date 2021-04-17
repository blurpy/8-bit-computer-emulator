#ifndef INC_8_BIT_COMPUTER_CLOCK_H
#define INC_8_BIT_COMPUTER_CLOCK_H

#include <thread>
#include <vector>

#include "ClockListener.h"
#include "TimeSource.h"

namespace Core {

    class Clock {

    public:
        explicit Clock(const std::shared_ptr<TimeSource> &timeSource);
        ~Clock();

        void start();
        virtual void stop();
        void join();
        void singleStep();
        void setFrequency(double hz);
        void addListener(const std::shared_ptr<ClockListener> &listener);
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
