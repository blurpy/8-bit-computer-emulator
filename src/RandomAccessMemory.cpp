#include <iostream>
#include <utility>

#include "Utils.h"

#include "RandomAccessMemory.h"

RandomAccessMemory::RandomAccessMemory(std::shared_ptr<Bus> bus) {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory in" << std::endl;
    }

    this->bus = std::move(bus);
    this->address = 0;
    this->readOnClock = false;
}

RandomAccessMemory::~RandomAccessMemory() {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory out" << std::endl;
    }
}

void RandomAccessMemory::readFromBus() {
    uint8_t busValue = bus->read();
    uint8_t currentValue = memory[address];

    if (Utils::debug()) {
        std::cout << "RandomAccessMemory: changing value from " << (int) currentValue << " to " << (int) busValue
                  << " at address " << (int) address << std::endl;
    }

    memory[address] = busValue;
}

void RandomAccessMemory::writeToBus() {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory: writing to bus " << (int) memory[address] << std::endl;
    }

    bus->write(memory[address]);
}

void RandomAccessMemory::print() {
    printf("RandomAccessMemory: current address - %d / 0x%02X / " BINARY_PATTERN " \n", address, address, BYTE_TO_BINARY(address));
    printf("RandomAccessMemory: current value - %d / 0x%02X / " BINARY_PATTERN " \n", memory[address], memory[address], BYTE_TO_BINARY(memory[address]));

    for (int i = 0; i < sizeof(memory); i++) {
        printf("RandomAccessMemory: value at %d - %d / 0x%02X / " BINARY_PATTERN " \n", i, memory[i], memory[i], BYTE_TO_BINARY(memory[i]));
    }
}

void RandomAccessMemory::reset() {
    address = 0;
}

void RandomAccessMemory::program(std::bitset<4> opcode, std::bitset<4> operand) {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory: programming at address " << (int) address << " with opcode " << opcode
                  << " and operand " << operand << std::endl;
    }

    std::bitset<8> newValue(opcode.to_string() + operand.to_string());
        memory[address] = newValue.to_ulong();
}

void RandomAccessMemory::in() {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void RandomAccessMemory::out() {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory: out" << std::endl;
    }

    writeToBus();
}

void RandomAccessMemory::clockTicked() {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

void RandomAccessMemory::registerValueChanged(uint8_t newValue) {
    if (Utils::debug()) {
        std::cout << "RandomAccessMemory: registerValueChanged. "
                  << "changing address from " << (int) address << " to " << (int) newValue << std::endl;
    }

    address = newValue;
}
