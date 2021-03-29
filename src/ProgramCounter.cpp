#include <iostream>

#include "Utils.h"

#include "ProgramCounter.h"

ProgramCounter::ProgramCounter(std::shared_ptr<Bus> bus) {
    std::cout << "ProgramCounter in" << std::endl;
    this->bus = std::move(bus);
    this->value = 0;
}

ProgramCounter::~ProgramCounter() {
    std::cout << "ProgramCounter out" << std::endl;
}

void ProgramCounter::increment() {
    value = ++value % 16;
    std::cout << "ProgramCounter: incremented to " << (int) value << std::endl;
}

void ProgramCounter::readFromBus() {
    uint8_t busValue = bus->read();
    std::cout << "ProgramCounter: changing value from " << (int) value << " to " << (int) busValue << std::endl;
    value = busValue;
}

void ProgramCounter::writeToBus() {
    std::cout << "ProgramCounter: writing to bus " << (int) value << std::endl;
    bus->write(value);
}

void ProgramCounter::print() {
    printf("ProgramCounter: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void ProgramCounter::reset() {
    value = 0;
}
