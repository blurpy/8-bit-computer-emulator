#include <iostream>

#include "Utils.h"

#include "InstructionRegister.h"

InstructionRegister::InstructionRegister(std::shared_ptr<Bus> bus) {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister in" << std::endl;
    }

    this->bus = std::move(bus);
    this->value = 0;
    this->readOnClock = false;
}

InstructionRegister::~InstructionRegister() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister out" << std::endl;
    }
}

void InstructionRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: read from bus. Changing value from " << (int) value << " to "
                  << (int) busValue << std::endl;
    }

    value = busValue;
}

void InstructionRegister::writeToBus() {
    uint8_t operand = value & 0x0F; // Extract the last 4 bits
    bus->write(operand);
}

void InstructionRegister::print() const {
    printf("InstructionRegister: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void InstructionRegister::reset() {
    value = 0;
}

void InstructionRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void InstructionRegister::out() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: out" << std::endl;
    }

    writeToBus();
}

void InstructionRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

uint8_t InstructionRegister::getOpcode() const {
    return value >> 4; // Extract the first 4 bits;
}
