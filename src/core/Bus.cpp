#include <iostream>

#include "Utils.h"

#include "Bus.h"

Core::Bus::Bus() {
    if (Utils::debugL2()) {
        std::cout << "Bus construct" << std::endl;
    }

    this->value = 0;
}

Core::Bus::~Bus() {
    if (Utils::debugL2()) {
        std::cout << "Bus destruct" << std::endl;
    }
}

uint8_t Core::Bus::read() const {
    return value;
}

void Core::Bus::write(const uint8_t newValue) {
    if (Utils::debugL2()) {
        std::cout << "Bus: changing value from " << (int) value << " to " << (int) newValue << std::endl;
    }

    value = newValue;

    notifyObserver();
}

void Core::Bus::print() const {
    printf("Bus: %d / 0x%02X / " BYTE_PATTERN "\n", value, value, BYTE_TO_BINARY(value));
}

void Core::Bus::reset() {
    value = 0;

    notifyObserver();
}

void Core::Bus::notifyObserver() const {
    if (observer != nullptr) {
        observer->valueUpdated(value);
    }
}

void Core::Bus::setObserver(const std::shared_ptr<ValueObserver> &newObserver) {
    observer = newObserver;
}
