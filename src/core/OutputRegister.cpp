#include <iostream>

#include "Utils.h"

#include "OutputRegister.h"

Core::OutputRegister::OutputRegister(const std::shared_ptr<Bus> &bus) {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister construct" << std::endl;
    }

    this->bus = bus;
    this->value = 0;
    this->readOnClock = false;
}

Core::OutputRegister::~OutputRegister() {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister destruct" << std::endl;
    }
}

void Core::OutputRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << "OutputRegister: read from bus. Changing value from " << (int) value << " to " << (int) busValue
                  << std::endl;
    }

    value = busValue;

    std::cout << "*** Display: " << (int) value << std::endl;

    notifyObserver();
}

void Core::OutputRegister::print() const {
    printf("OutputRegister: %d / 0x%02X / " BYTE_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void Core::OutputRegister::reset() {
    value = 0;

    notifyObserver();
}

void Core::OutputRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Core::OutputRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

void Core::OutputRegister::notifyObserver() const {
    if (observer != nullptr) {
        observer->valueUpdated(value);
    }
}

void Core::OutputRegister::setObserver(const std::shared_ptr<ValueObserver> &newObserver) {
    observer = newObserver;
}
