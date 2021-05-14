#include <bitset>
#include <iostream>

#include "../core/Utils.h"

#include "ArithmeticLogicUnitModel.h"

UI::ArithmeticLogicUnitModel::ArithmeticLogicUnitModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "ArithmeticLogicUnitModel construct" << std::endl;
    }

    this->value = 0;
    this->carry = false;
    this->zero = true;
}

UI::ArithmeticLogicUnitModel::~ArithmeticLogicUnitModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "ArithmeticLogicUnitModel destruct" << std::endl;
    }
}

void UI::ArithmeticLogicUnitModel::resultUpdated(const uint8_t newValue, const bool newCarryBit, const bool newZeroBit) {
    value = newValue;
    carry = newCarryBit;
    zero = newZeroBit;
}

std::string UI::ArithmeticLogicUnitModel::getRenderText() const {
    return "Arithmetic Logic Unit: " +
           std::bitset<8>(value).to_string() + " / " + std::to_string(value) +
           " C=" + std::to_string(carry) + " Z=" + std::to_string(zero);
}
