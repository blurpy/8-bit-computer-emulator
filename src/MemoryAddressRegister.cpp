#include <cassert>
#include <iostream>
#include <utility>

#include "Utils.h"

#include "MemoryAddressRegister.h"

MemoryAddressRegister::MemoryAddressRegister(std::shared_ptr<RandomAccessMemory> ram, std::shared_ptr<Bus> bus) {
    std::cout << "MemoryAddressRegister in" << std::endl;
    this->ram = std::move(ram);
    this->bus = std::move(bus);
    this->value = 0;
    this->readOnClock = false;
}

MemoryAddressRegister::~MemoryAddressRegister() {
    std::cout << "MemoryAddressRegister out" << std::endl;
}

void MemoryAddressRegister::readFromBus() {
    uint8_t busValue = bus->read();
    std::cout << "MemoryAddressRegister: read from bus. Changing value from " << (int) value << " to " << (int) busValue << std::endl;
    assert(busValue <= 15); // 4 bits only
    value = busValue;
    ram->setAddress(value);
}

void MemoryAddressRegister::print() const {
    printf("MemoryAddressRegister: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void MemoryAddressRegister::reset() {
    value = 0;
}

void MemoryAddressRegister::program(uint8_t newValue) {
    std::cout << "MemoryAddressRegister: programming at address " << (int) newValue << std::endl;
    value = newValue;
    ram->setAddress(value);
}

void MemoryAddressRegister::program(std::bitset<4> address) {
    std::cout << "MemoryAddressRegister: programming at address " << address << std::endl;
    value = address.to_ulong();
    ram->setAddress(value);
}

void MemoryAddressRegister::in() {
    std::cout << "MemoryAddressRegister: in - will read from bus on clock tick" << std::endl;
    readOnClock = true;
}

void MemoryAddressRegister::clockTicked() {
    std::cout << "MemoryAddressRegister: clock ticked" << std::endl;

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}
