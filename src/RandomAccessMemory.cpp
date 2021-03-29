#include <iostream>
#include <utility>

#include "Utils.h"

#include "RandomAccessMemory.h"

RandomAccessMemory::RandomAccessMemory(std::shared_ptr<Bus> bus) {
    std::cout << "RandomAccessMemory in" << std::endl;
    this->bus = std::move(bus);
    this->address = 0;
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
    printf("RandomAccessMemory: %d / 0x%02X / " BINARY_PATTERN " \n", memory[address], memory[address], BYTE_TO_BINARY(memory[address]));
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
