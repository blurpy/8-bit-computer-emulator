#include <iostream>

#include "Utils.h"

#include "InstructionRegister.h"

Core::InstructionRegister::InstructionRegister(const std::shared_ptr<Bus> &bus) {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister construct" << std::endl;
    }

    this->bus = bus;
    this->value = 0;
    this->readOnClock = false;
}

Core::InstructionRegister::~InstructionRegister() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister destruct" << std::endl;
    }
}

void Core::InstructionRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: read from bus. Changing value from " << (int) value << " to "
                  << (int) busValue << std::endl;
    }

    value = busValue;

    notifyObserver();
}

void Core::InstructionRegister::writeToBus() {
    uint8_t operand = value & 0x0F; // Extract the last 4 bits
    bus->write(operand);
}

void Core::InstructionRegister::print() const {
    printf("InstructionRegister: %d / 0x%02X / " BYTE_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void Core::InstructionRegister::reset() {
    value = 0;

    notifyObserver();
}

void Core::InstructionRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Core::InstructionRegister::out() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: out" << std::endl;
    }

    writeToBus();
}

void Core::InstructionRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "InstructionRegister: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

uint8_t Core::InstructionRegister::getOpcode() const {
    return value >> 4; // Extract the first 4 bits;
}

void Core::InstructionRegister::notifyObserver() const {
    if (observer != nullptr) {
        observer->valueUpdated(value);
    }
}

void Core::InstructionRegister::setObserver(const std::shared_ptr<ValueObserver> &newObserver) {
    observer = newObserver;
}
