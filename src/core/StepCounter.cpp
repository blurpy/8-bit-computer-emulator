#include <iostream>

#include "Utils.h"

#include "StepCounter.h"

Core::StepCounter::StepCounter(const std::shared_ptr<StepListener> &stepListener) {
    if (Utils::debugL2()) {
        std::cout << "StepCounter construct" << std::endl;
    }

    this->stepListener = stepListener;
    this->counter = 0;
    this->init = true;
}

Core::StepCounter::~StepCounter() {
    if (Utils::debugL2()) {
        std::cout << "StepCounter destruct" << std::endl;
    }
}

void Core::StepCounter::print() const {
    printf("StepCounter: %d / 0x%02X / " BIT_3_PATTERN " \n", counter, counter, BIT_3_TO_BINARY(counter));
}

void Core::StepCounter::reset() {
    counter = 0;
    init = true;
}

void Core::StepCounter::increment() {
    if (init) {
        init = false;
    } else {
        counter = ++counter % 5;
    }

    if (Utils::debugL2()) {
        std::cout << "StepCounter: incremented to " << (int) counter << std::endl;
    }
}

void Core::StepCounter::invertedClockTicked() {
    if (Utils::debugL2()) {
        std::cout << "StepCounter: inverted clock ticked" << std::endl;
    }

    increment();
    stepListener->stepReady(counter);
}
