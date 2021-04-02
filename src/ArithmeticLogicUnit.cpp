#include <iostream>
#include <utility>

#include "Utils.h"

#include "ArithmeticLogicUnit.h"

ArithmeticLogicUnit::ArithmeticLogicUnit(std::shared_ptr<Register> aRegister, std::shared_ptr<Register> bRegister, std::shared_ptr<Bus> bus) {
    if (Utils::debug()) {
        std::cout << "ArithmeticLogicUnit in" << std::endl;
    }

    this->aRegister = std::move(aRegister);
    this->bRegister = std::move(bRegister);
    this->bus = std::move(bus);
    this->value = 0;
    this->carry = false;
    this->zero = true;
}

ArithmeticLogicUnit::~ArithmeticLogicUnit() {
    if (Utils::debug()) {
        std::cout << "ArithmeticLogicUnit out" << std::endl;
    }
}

void ArithmeticLogicUnit::writeToBus() {
    this->bus->write(this->value);
}

void ArithmeticLogicUnit::print() const {
    printf("ArithmeticLogicUnit: value - %d / 0x%02X / " BINARY_PATTERN " \n", this->value, this->value, BYTE_TO_BINARY(this->value));
    std::cout << "ArithmeticLogicUnit: bits - C=" << this->carry << ", Z=" << this->zero << std::endl;
}

void ArithmeticLogicUnit::reset() {
    value = 0;
}

void ArithmeticLogicUnit::out() {
    if (Utils::debug()) {
        std::cout << "ArithmeticLogicUnit: out" << std::endl;
    }

    writeToBus();
}

void ArithmeticLogicUnit::registerValueChanged(uint8_t registerValue) {
    add();
}

void ArithmeticLogicUnit::add() {
    uint8_t aValue = aRegister->readValue();
    uint8_t bValue = bRegister->readValue();
    uint16_t result = aValue + bValue;
    uint8_t newValue = result;
    bool newCarry = result > 255;
    bool newZero = newValue == 0; // Both can be active at once if result is 256 (0b100000000) / new value is 0

    if (Utils::debug()) {
        std::cout << "ArithmeticLogicUnit: add. changing value from " << (int) value << " to " << (int) result
                  << " (" << (int) newValue << ")" << std::endl;
        std::cout << "ArithmeticLogicUnit: add. changing bits from C=" << carry << ", Z=" << zero
                  << " to C=" << newCarry << ", Z=" << newZero << std::endl;
    }

    value = newValue;
    carry = newCarry;
    zero = newZero;
}

/**
 * Subtracts using two's compliment.
 *
 * Example: 30 - 12
 * 30  = 0001 1110
 * 12  = 0000 1100
 *
 * Since the computer only does addition, we can convert 12 to -12 using two's compliment, and then
 * think of the calculation as 30 + -12.
 *
 * Two's compliment of 12 is done by inverting the bits and adding 1.
 * Inverted 12 = 1111 0011
 *          +1 = 1111 0100
 *             = 244
 *
 * The calculation then becomes 30 + 244 = 274
 * 274 = 1 0001 0010
 * Or 18 (0001 0010) + the carry bit
 *
 * This is why the carry bit LED is often on when subtracting.
 *
 * The carry bit is not set when the result is 255 and less.
 *
 * Example: 0 - 1
 * 0 = 0000 0000
 * 1 = 0000 0001
 * Inverted 1 = 1111 1110
 *         +1 = 1111 1111
 *            = 255
 * 0 + 255 = 255 (no carry needed)
 */
void ArithmeticLogicUnit::subtract() {
    uint8_t aValue = aRegister->readValue();
    uint8_t bValue = -(unsigned int) bRegister->readValue(); // Two's complement conversion
    uint16_t result = aValue + bValue;
    uint8_t newValue = result;
    bool newCarry = result > 255;
    bool newZero = newValue == 0; // Both can be active at once if result is 256 (0b100000000) / new value is 0

    if (Utils::debug()) {
        std::cout << "ArithmeticLogicUnit: subtract. changing value from " << (int) value << " to " << (int) result
                  << " (" << (int) newValue << ")" << std::endl;
        std::cout << "ArithmeticLogicUnit: subtract. changing bits from C=" << carry << ", Z=" << zero
                  << " to C=" << newCarry << ", Z=" << newZero << std::endl;
    }

    value = newValue;
    carry = newCarry;
    zero = newZero;
}

bool ArithmeticLogicUnit::isCarry() const {
    return carry;
}

bool ArithmeticLogicUnit::isZero() const {
    return zero;
}
