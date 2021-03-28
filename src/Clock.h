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
    void singleStep();
    void setFrequency(double hz);
    void addListener(ClockListener* listener);

private:
    double frequency;
    double counter;
    bool running;
    bool rising;
    std::chrono::steady_clock::time_point lastTick;
    std::thread clockThread;
    std::vector<ClockListener*> listeners;

    void mainLoop();
    bool tick();
    void sleep(int milliseconds) const;
};

#endif //INC_8_BIT_COMPUTER_CLOCK_H
