#include <iostream>
#include <utility>

#include "Utils.h"

#include "ArithmeticLogicUnit.h"

ArithmeticLogicUnit::ArithmeticLogicUnit(std::shared_ptr<Register> aRegister, std::shared_ptr<Register> bRegister, std::shared_ptr<Bus> bus) {
    std::cout << "ArithmeticLogicUnit in" << std::endl;
    this->aRegister = std::move(aRegister);
    this->bRegister = std::move(bRegister);
    this->bus = std::move(bus);
    this->value = 0;
}

ArithmeticLogicUnit::~ArithmeticLogicUnit() {
    std::cout << "ArithmeticLogicUnit out" << std::endl;
}

void ArithmeticLogicUnit::writeToBus() {
    uint8_t aValue = this->aRegister->readValue();
    uint8_t bValue = this->bRegister->readValue();
    uint8_t result = aValue + bValue;

    std::cout << "ArithmeticLogicUnit: changing value from " << (int) this->value << " to " << (int) result << std::endl;

    this->value = result;
    this->bus->write(this->value);
}

void ArithmeticLogicUnit::print() const {
    printf("ArithmeticLogicUnit: %d / 0x%02X / " BINARY_PATTERN " \n", this->value, this->value, BYTE_TO_BINARY(this->value));
}
