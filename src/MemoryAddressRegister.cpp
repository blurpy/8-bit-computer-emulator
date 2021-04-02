#include <cassert>
#include <iostream>
#include <utility>

#include "Utils.h"

#include "MemoryAddressRegister.h"

MemoryAddressRegister::MemoryAddressRegister(std::shared_ptr<RegisterListener> registerListener, std::shared_ptr<Bus> bus) {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister construct" << std::endl;
    }

    this->registerListener = std::move(registerListener);
    this->bus = std::move(bus);
    this->value = 0;
    this->readOnClock = false;
}

MemoryAddressRegister::~MemoryAddressRegister() {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister destruct" << std::endl;
    }
}

void MemoryAddressRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: read from bus. Changing value from " << (int) value << " to "
                  << (int) busValue << std::endl;
    }

    assert(busValue <= 15); // 4 bits only
    value = busValue;
    registerListener->registerValueChanged(value);
}

void MemoryAddressRegister::print() const {
    printf("MemoryAddressRegister: %d / 0x%02X / " BINARY_PATTERN " \n", value, value, BYTE_TO_BINARY(value));
}

void MemoryAddressRegister::reset() {
    value = 0;
}

void MemoryAddressRegister::program(std::bitset<4> address) {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: programming at address " << address << std::endl;
    }

    value = address.to_ulong();
    registerListener->registerValueChanged(value);
}

void MemoryAddressRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void MemoryAddressRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}
