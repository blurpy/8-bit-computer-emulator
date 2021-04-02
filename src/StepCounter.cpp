#include <iostream>
#include <utility>

#include "Utils.h"

#include "StepCounter.h"

StepCounter::StepCounter(std::shared_ptr<StepListener> stepListener) {
    std::cout << "StepCounter in" << std::endl;
    this->stepListener = std::move(stepListener);
    this->counter = 0;
    this->init = true;
}

StepCounter::~StepCounter() {
    std::cout << "StepCounter out" << std::endl;
}

void StepCounter::print() const {
    printf("StepCounter: %d / 0x%02X / " BINARY_PATTERN " \n", counter, counter, BYTE_TO_BINARY(counter));
}

void StepCounter::reset() {
    counter = 0;
    init = true;
}

void StepCounter::increment() {
    if (init) {
        init = false;
    } else {
        counter = ++counter % 5;
    }

    if (Utils::debug()) {
        std::cout << "StepCounter: incremented to " << (int) counter << std::endl;
    }
}

void StepCounter::invertedClockTicked() {
    if (Utils::debug()) {
        std::cout << "StepCounter: inverted clock ticked" << std::endl;
    }

    increment();
    stepListener->stepReady(counter);
}
