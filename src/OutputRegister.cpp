#include <iostream>
#include <utility>

#include "Utils.h"

#include "OutputRegister.h"

OutputRegister::OutputRegister(std::shared_ptr<Bus> bus) {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister construct" << std::endl;
    }

    this->bus = std::move(bus);
    this->value = 0;
    this->readOnClock = false;
}

OutputRegister::~OutputRegister() {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister out" << std::endl;
    }
}

void OutputRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << "OutputRegister: read from bus. Changing value from " << (int) value << " to " << (int) busValue
                  << std::endl;
    }

    value = busValue;

    std::cout << "*** Display: " << (int) value << std::endl;
}

void OutputRegister::print() const {
    printf("OutputRegister: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void OutputRegister::reset() {
    value = 0;
}

void OutputRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void OutputRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "OutputRegister: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}
