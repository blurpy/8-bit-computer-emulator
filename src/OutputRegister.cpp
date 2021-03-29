#include <iostream>
#include <utility>

#include "Utils.h"

#include "OutputRegister.h"

OutputRegister::OutputRegister(std::shared_ptr<Bus> bus) {
    std::cout << "OutputRegister in" << std::endl;
    this->bus = std::move(bus);
    this->value = 0;
}

OutputRegister::~OutputRegister() {
    std::cout << "OutputRegister out" << std::endl;
}

void OutputRegister::readFromBus() {
    uint8_t busValue = bus->read();
    std::cout << "OutputRegister: read from bus. Changing value from " << (int) value << " to " << (int) busValue << std::endl;
    value = busValue;
}

uint8_t OutputRegister::readValue() const {
    return this->value;
}

void OutputRegister::print() {
    printf("OutputRegister: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void OutputRegister::reset() {
    value = 0;
}
