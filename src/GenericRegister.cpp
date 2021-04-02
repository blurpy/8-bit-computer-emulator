#include <iostream>
#include <utility>

#include "Utils.h"

#include "GenericRegister.h"

GenericRegister::GenericRegister(const std::string& name, std::shared_ptr<Bus> bus) {
    this->name = name;
    this->bus = std::move(bus);
    this->value = 0;
    this->readOnClock = false;

    if (Utils::debugL2()) {
        std::cout << this->name << " register in" << std::endl;
    }
}

GenericRegister::~GenericRegister() {
    if (Utils::debugL2()) {
        std::cout << this->name << " register out" << std::endl;
    }
}

void GenericRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << this->name << " register: changing value from " << (int) this->value << " to " << (int) busValue << std::endl;
    }

    this->value = busValue;

    if (registerListener != nullptr) {
        registerListener->registerValueChanged(value);
    }
}

void GenericRegister::writeToBus() {
    this->bus->write(this->value);
}

uint8_t GenericRegister::readValue() const {
    return this->value;
}

void GenericRegister::print() {
    printf("%s register: %d / 0x%02X / " BINARY_PATTERN " \n", this->name.c_str(), this->value, this->value, BYTE_TO_BINARY(this->value));
}

void GenericRegister::reset() {
    value = 0;
}

void GenericRegister::in() {
    if (Utils::debugL2()) {
        std::cout << this->name << " register: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void GenericRegister::out() {
    if (Utils::debugL2()) {
        std::cout << this->name << " register: out" << std::endl;
    }

    writeToBus();
}

void GenericRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << this->name << " register: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

void GenericRegister::setRegisterListener(std::shared_ptr<RegisterListener> newRegisterListener) {
    this->registerListener = std::move(newRegisterListener);
}
