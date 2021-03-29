#include <iostream>

#include "Utils.h"

#include "InstructionRegister.h"

InstructionRegister::InstructionRegister(std::shared_ptr<Bus> bus) {
    std::cout << "InstructionRegister in" << std::endl;
    this->bus = std::move(bus);
    this->value = 0;
}

InstructionRegister::~InstructionRegister() {
    std::cout << "InstructionRegister out" << std::endl;
}

void InstructionRegister::readFromBus() {
    uint8_t busValue = bus->read();
    std::cout << "InstructionRegister: read from bus. Changing value from " << (int) value << " to " << (int) busValue << std::endl;
    value = busValue;
}

void InstructionRegister::writeToBus() {
    bus->write(value);
}

void InstructionRegister::print() {
    printf("InstructionRegister: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void InstructionRegister::reset() {
    value = 0;
}
