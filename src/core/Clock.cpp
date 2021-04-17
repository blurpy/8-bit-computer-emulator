#include "cassert"
#include <cmath>
#include <iostream>

#include "Utils.h"

#include "Clock.h"

Core::Clock::Clock(const std::shared_ptr<TimeSource> &timeSource) {
    if (Utils::debugL2()) {
        std::cout << "Clock construct" << std::endl;
    }

    this->timeSource = timeSource;
    this->counter = 0;
    this->frequency = 0;
    this->running = false;
    this->rising = false;
    this->singleStepping = false;
    this->remainingTicks = 0;
}

Core::Clock::~Clock() {
    if (Utils::debugL2()) {
        std::cout << "Clock destruct" << std::endl;
    }

    running = false;
    listeners.clear();
}

void Core::Clock::start() {
    std::cout << "Clock: starting clock" << std::endl;
    assert(frequency > 0);

    counter = 0;
    running = true;
    rising = true;
    singleStepping = false;
    timeSource->reset();
    clockThread = std::thread(&Clock::mainLoop, this);
}

void Core::Clock::stop() {
    running = false;
    std::cout << "Clock: stopped" << std::endl;
}

void Core::Clock::join() {
    clockThread.join();
}

void Core::Clock::singleStep() {
    std::cout << "Clock: single stepping clock" << std::endl;
    assert(frequency > 0);

    counter = 0;
    running = true;
    rising = true;
    singleStepping = true;
    remainingTicks = 2;
    timeSource->reset();
    clockThread = std::thread(&Clock::mainLoop, this);
    clockThread.join();
}

void Core::Clock::setFrequency(const double hz) {
    frequency = (1.0 / (hz * 2.0) * 1000.0 * 1000.0 * 1000.0);
}

void Core::Clock::mainLoop() {
    if (Utils::debugL1()) {
        std::cout << "Clock: starting main loop" << std::endl;
    }

    while (running) {
        if (tick()) {
            if (rising) {
                for (auto &listener : listeners) {
                    listener->clockTicked();
                }
                rising = false;
            }

            else {
                for (auto &listener : listeners) {
                    listener->invertedClockTicked();
                }
                rising = true;
            }

            if (singleStepping && --remainingTicks <= 0) {
                running = false;
            }
        }

        timeSource->sleep(100);
    }

    if (Utils::debugL1()) {
        std::cout << "Clock: exiting main loop" << std::endl;
    }
}

bool Core::Clock::tick() {
    bool incremented = false;

    counter += timeSource->delta();

    if (counter >= frequency) {
        incremented = true;
        counter = std::fmod(counter, frequency);
    }

    return incremented;
}

void Core::Clock::addListener(const std::shared_ptr<ClockListener> &listener) {
    listeners.push_back(listener);
}

void Core::Clock::clearListeners() {
    listeners.clear();
}
