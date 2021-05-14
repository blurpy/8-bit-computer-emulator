#include <iostream>
#include <string>

#include "Utils.h"

#include "RandomAccessMemory.h"

Core::RandomAccessMemory::RandomAccessMemory(const std::shared_ptr<Bus> &bus) {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory construct" << std::endl;
    }

    this->bus = bus;
    this->address = 0;
    this->readOnClock = false;
}

Core::RandomAccessMemory::~RandomAccessMemory() {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory destruct" << std::endl;
    }
}

void Core::RandomAccessMemory::readFromBus() {
    uint8_t busValue = bus->read();
    uint8_t currentValue = memory[address];

    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory: changing value from " << (int) currentValue << " to " << (int) busValue
                  << " at address " << (int) address << std::endl;
    }

    memory[address] = busValue;

    notifyObserver();
}

void Core::RandomAccessMemory::writeToBus() {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory: writing to bus " << (int) memory[address] << std::endl;
    }

    bus->write(memory[address]);
}

void Core::RandomAccessMemory::print() {
    printf("RandomAccessMemory: current address - %d / 0x%02X / " BIT_4_PATTERN " \n", address, address, BIT_4_TO_BINARY(address));
    printf("RandomAccessMemory: current value - %d / 0x%02X / " BYTE_PATTERN " \n", memory[address], memory[address], BYTE_TO_BINARY(memory[address]));

    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("RandomAccessMemory: value at %d - %d / 0x%02X / " BYTE_PATTERN " \n", i, memory[i], memory[i], BYTE_TO_BINARY(memory[i]));
    }
}

void Core::RandomAccessMemory::reset() {
    address = 0;
}

void Core::RandomAccessMemory::program(const std::bitset<4> &opcode, const std::bitset<4> &operand) {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory: programming at address " << (int) address << " with opcode " << opcode
                  << " and operand " << operand << std::endl;
    }

    std::bitset<8> newValue(opcode.to_string() + operand.to_string());
    memory[address] = newValue.to_ulong();

    notifyObserver();
}

void Core::RandomAccessMemory::in() {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Core::RandomAccessMemory::out() {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory: out" << std::endl;
    }

    writeToBus();
}

void Core::RandomAccessMemory::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

void Core::RandomAccessMemory::registerValueChanged(const uint8_t newValue) {
    if (Utils::debugL2()) {
        std::cout << "RandomAccessMemory: registerValueChanged. "
                  << "changing address from " << (int) address << " to " << (int) newValue << std::endl;
    }

    if (newValue >= MEMORY_SIZE) {
        throw std::runtime_error("RandomAccessMemory: address out of bounds " + std::to_string(newValue));
    }

    address = newValue;

    notifyObserver();
}

void Core::RandomAccessMemory::notifyObserver() const {
    if (observer != nullptr) {
        observer->valueUpdated(memory[address]);
    }
}

void Core::RandomAccessMemory::setObserver(const std::shared_ptr<ValueObserver> &newObserver) {
    observer = newObserver;
}
