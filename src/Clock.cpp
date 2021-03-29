#include "cassert"
#include <cmath>
#include <iostream>

#include "Clock.h"

Clock::Clock() {
    std::cout << "Clock in" << std::endl;
    this->counter = 0;
    this->frequency = 0;
    this->running = false;
    this->rising = false;
    this->singleStepping = false;
    this->remainingTicks = 0;
}

Clock::~Clock() {
    std::cout << "Clock out" << std::endl;
    running = false;
    listeners.clear();
}

void Clock::start() {
    std::cout << "Clock: starting clock" << std::endl;
    assert(frequency > 0);

    counter = 0;
    running = true;
    rising = true;
    singleStepping = false;
    lastTick = std::chrono::steady_clock::now();
    clockThread = std::thread(&Clock::mainLoop, this);
}

void Clock::stop() {
    running = false;
    clockThread.join();
    std::cout << "Clock: stopped" << std::endl;
}

void Clock::singleStep() {
    std::cout << "Clock: single stepping clock" << std::endl;
    assert(frequency > 0);

    counter = 0;
    running = true;
    rising = true;
    singleStepping = true;
    remainingTicks = 2;
    lastTick = std::chrono::steady_clock::now();
    clockThread = std::thread(&Clock::mainLoop, this);
    clockThread.join();
}

void Clock::setFrequency(double hz) {
    frequency = (1.0 / (hz * 2.0) * 1000.0 * 1000.0 * 1000.0);
}

void Clock::mainLoop() {
    std::cout << "Clock: starting main loop" << std::endl;

    while (running) {
        if (tick()) {
            if (rising) {
                for (auto & listener : listeners) {
                    listener->clockTicked();
                }
                rising = false;
            }

            else {
                for (auto & listener : listeners) {
                    listener->invertedClockTicked();
                }
                rising = true;
            }

            if (singleStepping && --remainingTicks <= 0) {
                running = false;
            }
        }

        sleep(1);
    }

    std::cout << "Clock: exiting main loop" << std::endl;
}

bool Clock::tick() {
    bool incremented = false;

    auto currentTick = std::chrono::steady_clock::now();
    auto timeSinceLastTick = currentTick - lastTick;
    auto timeSinceLastTickNano = std::chrono::duration_cast<std::chrono::nanoseconds>(timeSinceLastTick);

    lastTick = currentTick;
    counter += timeSinceLastTickNano.count();

    if (counter > frequency) {
        incremented = true;
        counter = std::fmod(counter, frequency);
    }

    return incremented;
}

void Clock::sleep(int milliseconds) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Clock::addListener(ClockListener* listener) {
    listeners.push_back(listener);
}
