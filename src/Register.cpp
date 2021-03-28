#include <iostream>
#include <utility>

#include "Utils.h"

#include "Register.h"

Register::Register(const std::string& name, std::shared_ptr<Bus> bus) {
    this->name = name;
    this->bus = std::move(bus);
    this->value = 0;
    std::cout << this->name << " register in" << std::endl;
}

Register::~Register() {
    std::cout << this->name << " register out" << std::endl;
}

void Register::readFromBus() {
    uint8_t busValue = bus->read();
    std::cout << this->name << " register: changing value from " << (int) this->value << " to " << (int) busValue << std::endl;
    this->value = busValue;
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
