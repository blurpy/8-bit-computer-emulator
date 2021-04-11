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
    return this->value;
}

void Core::Bus::write(const uint8_t newValue) {
    if (Utils::debugL2()) {
        std::cout << "Bus: changing value from " << (int) this->value << " to " << (int) newValue << std::endl;
    }

    this->value = newValue;
}

void Core::Bus::print() const {
    printf("Bus: %d / 0x%02X / " BYTE_PATTERN "\n", this->value, this->value, BYTE_TO_BINARY(this->value));
}

void Core::Bus::reset() {
    value = 0;
}
