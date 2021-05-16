#include <iostream>

#include "Utils.h"

#include "StepCounter.h"

Core::StepCounter::StepCounter(const std::shared_ptr<StepListener> &stepListener) {
    if (Utils::debugL2()) {
        std::cout << "StepCounter construct" << std::endl;
    }

    this->stepListener = stepListener;
    this->counter = 0;
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

    notifyObserver();
    notifyListener();
}

void Core::StepCounter::increment() {
    counter = ++counter % 5;

    if (Utils::debugL2()) {
        std::cout << "StepCounter: incremented to " << (int) counter << std::endl;
    }

    notifyObserver();
    notifyListener();
}

void Core::StepCounter::invertedClockTicked() {
    if (Utils::debugL2()) {
        std::cout << "StepCounter: inverted clock ticked" << std::endl;
    }

    increment();
}

void Core::StepCounter::notifyObserver() const {
    if (observer != nullptr) {
        observer->valueUpdated(counter);
    }
}

void Core::StepCounter::notifyListener() const {
    stepListener->stepReady(counter);
}

void Core::StepCounter::setObserver(const std::shared_ptr<ValueObserver> &newObserver) {
    observer = newObserver;
}
