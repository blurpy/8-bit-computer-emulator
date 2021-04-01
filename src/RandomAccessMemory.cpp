#include <iostream>
#include <utility>

#include "Utils.h"

#include "RandomAccessMemory.h"

RandomAccessMemory::RandomAccessMemory(std::shared_ptr<Bus> bus) {
    std::cout << "RandomAccessMemory in" << std::endl;
    this->bus = std::move(bus);
    this->address = 0;
    this->readOnClock = false;
}

RandomAccessMemory::~RandomAccessMemory() {
    std::cout << "RandomAccessMemory out" << std::endl;
}

void RandomAccessMemory::readFromBus() {
    uint8_t busValue = bus->read();
    uint8_t currentValue = memory[address];

    std::cout << "RandomAccessMemory: changing value from " << (int) currentValue << " to " << (int) busValue << " at address " << (int) address << std::endl;
    memory[address] = busValue;
}

void RandomAccessMemory::writeToBus() {
    std::cout << "RandomAccessMemory: writing to bus " << (int) memory[address] << std::endl;
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

void RandomAccessMemory::setAddress(uint8_t newAddress) {
    std::cout << "RandomAccessMemory: changing address from " << (int) address << " to " << (int) newAddress << std::endl;
    address = newAddress;
}

void RandomAccessMemory::program(uint8_t newValue) {
    std::cout << "RandomAccessMemory: programming at address " << (int) address << " with value " << (int) newValue << std::endl;
    memory[address] = newValue;
}

void RandomAccessMemory::program(std::bitset<4> opcode, std::bitset<4> operand) {
    std::cout << "RandomAccessMemory: programming at address " << (int) address << " with opcode " << opcode << " and operand " << operand << std::endl;
    std::bitset<8> newValue(opcode.to_string() + operand.to_string());
        memory[address] = newValue.to_ulong();
}

void RandomAccessMemory::program(std::bitset<8> newValue) {
    std::cout << "RandomAccessMemory: programming at address " << (int) address << " with value " << newValue << std::endl;
    memory[address] = newValue.to_ulong();
}

void RandomAccessMemory::in() {
    std::cout << "RandomAccessMemory: in - will read from bus on clock tick" << std::endl;
    readOnClock = true;
}

void RandomAccessMemory::out() {
    std::cout << "RandomAccessMemory: out" << std::endl;
    writeToBus();
}

void RandomAccessMemory::clockTicked() {
    std::cout << "RandomAccessMemory: clock ticked" << std::endl;

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}
