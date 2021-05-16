#include <iostream>
#include <string>

#include "Utils.h"

#include "ProgramCounter.h"

Core::ProgramCounter::ProgramCounter(const std::shared_ptr<Bus> &bus) {
    if (Utils::debugL2()) {
        std::cout << "ProgramCounter construct" << std::endl;
    }

    this->bus = bus;
    this->value = 0;
    this->incrementOnClock = false;
    this->readOnClock = false;
}

Core::ProgramCounter::~ProgramCounter() {
    if (Utils::debugL2()) {
        std::cout << "ProgramCounter destruct" << std::endl;
    }
}

void Core::ProgramCounter::increment() {
    value = ++value % 16;

    if (Utils::debugL2()) {
        std::cout << "ProgramCounter: incremented to " << (int) value << std::endl;
    }

    notifyObserver();
}

void Core::ProgramCounter::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << "ProgramCounter: changing value from " << (int) value << " to " << (int) busValue << std::endl;
    }

    if (busValue > Utils::FOUR_BITS_MAX) {
        throw std::runtime_error("ProgramCounter: address out of bounds " + std::to_string(busValue));
    }

    value = busValue;

    notifyObserver();
}

void Core::ProgramCounter::writeToBus() {
    if (Utils::debugL2()) {
        std::cout << "ProgramCounter: writing to bus " << (int) value << std::endl;
    }

    bus->write(value);
}

void Core::ProgramCounter::print() const {
    printf("ProgramCounter: %d / 0x%02X / " BIT_4_PATTERN " \n", value, value, BIT_4_TO_BINARY(value));
}

void Core::ProgramCounter::reset() {
    value = 0;

    notifyObserver();
}

void Core::ProgramCounter::out() {
    if (Utils::debugL2()) {
        std::cout << "ProgramCounter: out" << std::endl;
    }

    writeToBus();
}

void Core::ProgramCounter::enable() {
    if (Utils::debugL2()) {
        std::cout << "ProgramCounter: enable - will increment on clock tick" << std::endl;
    }

    incrementOnClock = true;
}

void Core::ProgramCounter::jump() {
    if (Utils::debugL2()) {
        std::cout << "ProgramCounter: jump - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Core::ProgramCounter::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "ProgramCounter: clock ticked" << std::endl;
    }

    if (incrementOnClock) {
        increment();
        incrementOnClock = false;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

void Core::ProgramCounter::notifyObserver() const {
    if (observer != nullptr) {
        observer->valueUpdated(value);
    }
}

void Core::ProgramCounter::setObserver(const std::shared_ptr<ValueObserver> &newObserver) {
    observer = newObserver;
}
