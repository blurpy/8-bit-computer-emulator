#ifndef INC_8_BIT_COMPUTER_CLOCK_H
#define INC_8_BIT_COMPUTER_CLOCK_H

#include <chrono>
#include <thread>
#include <vector>

#include "ClockListener.h"

class Clock {

public:
    Clock();
    ~Clock();

    void start();
    void stop();
    void join();
    void singleStep();
    void setFrequency(double hz);
    void addListener(const std::shared_ptr<ClockListener> &listener);
    void clearListeners();

private:
    double frequency;
    double counter;
    bool running;
    bool rising;
    bool singleStepping;
    int remainingTicks;
    std::chrono::steady_clock::time_point lastTick;
    std::thread clockThread;
    std::vector<std::shared_ptr<ClockListener>> listeners;

    void mainLoop();
    bool tick();
    void sleep(int microseconds) const;
};

#endif //INC_8_BIT_COMPUTER_CLOCK_H
