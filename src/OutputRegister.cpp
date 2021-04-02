#include <iostream>
#include <utility>

#include "Utils.h"

#include "OutputRegister.h"

OutputRegister::OutputRegister(std::shared_ptr<Bus> bus) {
    std::cout << "OutputRegister in" << std::endl;
    this->bus = std::move(bus);
    this->value = 0;
    this->readOnClock = false;
}

OutputRegister::~OutputRegister() {
    std::cout << "OutputRegister out" << std::endl;
}

void OutputRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debug()) {
        std::cout << "OutputRegister: read from bus. Changing value from " << (int) value << " to " << (int) busValue
                  << std::endl;
    }

    value = busValue;
}

void OutputRegister::print() const {
    printf("OutputRegister: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void OutputRegister::reset() {
    value = 0;
}

void OutputRegister::in() {
    if (Utils::debug()) {
        std::cout << "OutputRegister: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void OutputRegister::clockTicked() {
    if (Utils::debug()) {
        std::cout << "OutputRegister: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}
