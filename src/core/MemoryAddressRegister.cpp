#include <cassert>
#include <iostream>

#include "Utils.h"

#include "MemoryAddressRegister.h"

Core::MemoryAddressRegister::MemoryAddressRegister(const std::shared_ptr<RegisterListener> &registerListener,
                                                   const std::shared_ptr<Bus> &bus) {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister construct" << std::endl;
    }

    this->registerListener = registerListener;
    this->bus = bus;
    this->value = 0;
    this->readOnClock = false;
}

Core::MemoryAddressRegister::~MemoryAddressRegister() {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister destruct" << std::endl;
    }
}

void Core::MemoryAddressRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: read from bus. Changing value from " << (int) value << " to "
                  << (int) busValue << std::endl;
    }

    assert(busValue <= 15); // 4 bits only
    value = busValue;
    registerListener->registerValueChanged(value);
}

void Core::MemoryAddressRegister::print() const {
    printf("MemoryAddressRegister: %d / 0x%02X / " BIT_4_PATTERN " \n", value, value, BIT_4_TO_BINARY(value));
}

void Core::MemoryAddressRegister::reset() {
    value = 0;
}

void Core::MemoryAddressRegister::program(const std::bitset<4> &address) {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: programming at address " << address << std::endl;
    }

    value = address.to_ulong();
    registerListener->registerValueChanged(value);
}

void Core::MemoryAddressRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Core::MemoryAddressRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "MemoryAddressRegister: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}
