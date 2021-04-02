#include <iostream>
#include <utility>

#include "Utils.h"

#include "Register.h"

Register::Register(const std::string& name, std::shared_ptr<Bus> bus) {
    this->name = name;
    this->bus = std::move(bus);
    this->value = 0;
    this->readOnClock = false;

    if (Utils::debug()) {
        std::cout << this->name << " register in" << std::endl;
    }
}

Register::~Register() {
    if (Utils::debug()) {
        std::cout << this->name << " register out" << std::endl;
    }
}

void Register::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debug()) {
        std::cout << this->name << " register: changing value from " << (int) this->value << " to " << (int) busValue << std::endl;
    }

    this->value = busValue;

    if (registerListener != nullptr) {
        registerListener->registerValueChanged(value);
    }
}

void Register::writeToBus() {
    this->bus->write(this->value);
}

uint8_t Register::readValue() const {
    return this->value;
}

void Register::print() {
    printf("%s register: %d / 0x%02X / " BINARY_PATTERN " \n", this->name.c_str(), this->value, this->value, BYTE_TO_BINARY(this->value));
}

void Register::reset() {
    value = 0;
}

void Register::in() {
    if (Utils::debug()) {
        std::cout << this->name << " register: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Register::out() {
    if (Utils::debug()) {
        std::cout << this->name << " register: out" << std::endl;
    }

    writeToBus();
}

void Register::clockTicked() {
    if (Utils::debug()) {
        std::cout << this->name << " register: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

void Register::setRegisterListener(std::shared_ptr<RegisterListener> newRegisterListener) {
    this->registerListener = std::move(newRegisterListener);
}
