#include <iostream>

#include "../core/Utils.h"

#include "FlagsRegisterModel.h"

UI::FlagsRegisterModel::FlagsRegisterModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "FlagsRegisterModel construct" << std::endl;
    }

    this->carryFlag = false;
    this->zeroFlag = false;
}

UI::FlagsRegisterModel::~FlagsRegisterModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "FlagsRegisterModel destruct" << std::endl;
    }
}

void UI::FlagsRegisterModel::flagsUpdated(const bool newCarryFlag, const bool newZeroFlag) {
    carryFlag = newCarryFlag;
    zeroFlag = newZeroFlag;
}

std::string UI::FlagsRegisterModel::getRenderText() const {
    return "Flags: C=" + std::to_string(carryFlag) + " Z=" + std::to_string(zeroFlag);
}
