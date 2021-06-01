#include <cmath>
#include <iostream>
#include <string>

#include "Utils.h"

#include "Clock.h"

Core::Clock::Clock(const std::shared_ptr<TimeSource> &timeSource) {
    if (Utils::debugL2()) {
        std::cout << "Clock construct" << std::endl;
    }

    this->timeSource = timeSource;
    this->counter = 0;
    this->frequency = 0;
    this->hz = 0;
    this->running = false;
    this->halted = false;
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

    if (halted) {
        std::cerr << "Clock: halted" << std::endl;
        return;
    }

    if (frequency <= 0) {
        throw std::runtime_error("Clock: frequency must be set before start");
    }

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

void Core::Clock::halt() {
    halted = true;
    stop();
}

void Core::Clock::join() {
    if (clockThread.joinable()) {
        clockThread.join();
    }
}

void Core::Clock::detach() {
    if (clockThread.joinable()) {
        clockThread.detach();
    }
}

void Core::Clock::singleStep() {
    std::cout << "Clock: single stepping clock" << std::endl;

    if (halted) {
        std::cerr << "Clock: halted" << std::endl;
        return;
    }

    if (running) {
        std::cerr << "Clock: already running" << std::endl;
        return;
    }

    if (frequency <= 0) {
        throw std::runtime_error("Clock: frequency must be set before start");
    }

    counter = 0;
    running = true;
    rising = true;
    singleStepping = true;
    remainingTicks = 2;
    timeSource->reset();
    clockThread = std::thread(&Clock::mainLoop, this);
    clockThread.join();
}

bool Core::Clock::isRunning() const {
    return running;
}

void Core::Clock::reset() {
    halted = false;
}

void Core::Clock::setFrequency(const double newHz) {
    if (Utils::debugL1()) {
        std::cout << "Clock: changing frequency to " << newHz << std::endl;
    }

    if (Utils::isLessThan(newHz, 0.1)) {
        throw std::runtime_error("Clock: frequency too low " + std::to_string(newHz));
    }

    hz = newHz;
    frequency = (1.0 / (hz * 2.0) * 1000.0 * 1000.0 * 1000.0);

    notifyFrequencyChanged();
}

void Core::Clock::increaseFrequency() {
    if (Utils::isLessThan(hz, 1)) {
        setFrequency(hz + 0.1);
    } else if (hz < 20) {
        setFrequency(hz + 1);
    } else if (hz < 200) {
        setFrequency(hz + 10);
    } else if (hz < 2000) {
        setFrequency(hz + 100);
    } else {
        setFrequency(hz + 1000);
    }
}

void Core::Clock::decreaseFrequency() {
    if (Utils::isLessThan(hz, 0.1) || Utils::equals(hz, 0.1)) {
        std::cerr << "Clock: can not decrease frequency below 0.1" << std::endl;
    } else if (hz <= 1) {
        setFrequency(hz - 0.1);
    } else if (hz <= 20) {
        setFrequency(hz - 1);
    } else if (hz <= 200) {
        setFrequency(hz - 10);
    } else if (hz <= 2000) {
        setFrequency(hz - 100);
    } else {
        setFrequency(hz - 1000);
    }
}

void Core::Clock::mainLoop() {
    if (Utils::debugL1()) {
        std::cout << "Clock: starting main loop" << std::endl;
    }

    while (running) {
        if (tick()) {
            if (rising) {
                notifyTick();
                rising = false;
            }

            else {
                notifyInvertedTick();
                rising = true;
            }

            if (singleStepping && --remainingTicks <= 0) {
                running = false;
            }
        }

        else {
            // Sleep the remaining time before the next tick
            timeSource->sleep(std::floor(frequency - counter));
        }
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

void Core::Clock::notifyTick() const {
    if (observer != nullptr) {
        observer->clockTicked(true);
    }

    for (auto &listener : listeners) {
        listener->clockTicked();
    }
}

void Core::Clock::notifyInvertedTick() const {
    if (observer != nullptr) {
        observer->clockTicked(false);
    }

    for (auto &listener : listeners) {
        listener->invertedClockTicked();
    }
}

void Core::Clock::notifyFrequencyChanged() const {
    if (observer != nullptr) {
        observer->frequencyChanged(hz);
    }
}

void Core::Clock::setObserver(const std::shared_ptr<ClockObserver> &newObserver) {
    observer = newObserver;
}
