#include <iostream>
#include <thread>

#include "Utils.h"

#include "TimeSource.h"

Core::TimeSource::TimeSource() {
    if (Utils::debugL2()) {
        std::cout << "TimeSource construct" << std::endl;
    }

    lastTime = std::chrono::steady_clock::now();
}

Core::TimeSource::~TimeSource() {
    if (Utils::debugL2()) {
        std::cout << "TimeSource destruct" << std::endl;
    }
}

void Core::TimeSource::reset() {
    lastTime = std::chrono::steady_clock::now();
}

double Core::TimeSource::delta() {
    auto currentTime = std::chrono::steady_clock::now();
    auto delta = currentTime - lastTime;

    lastTime = currentTime;

    return delta.count();
}

void Core::TimeSource::sleep(const long nanoseconds) const {
    std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
}
