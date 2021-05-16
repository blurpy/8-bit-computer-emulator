#include <iostream>

#include "Utils.h"

#include "FlagsRegister.h"

Core::FlagsRegister::FlagsRegister(const std::shared_ptr<ArithmeticLogicUnit> &arithmeticLogicUnit) {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister construct" << std::endl;
    }

    this->arithmeticLogicUnit = arithmeticLogicUnit;
    this->readOnClock = false;
    this->carryFlag = false;
    this->zeroFlag = false;
}

Core::FlagsRegister::~FlagsRegister() {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister destruct" << std::endl;
    }
}

void Core::FlagsRegister::print() const {
    std::cout << "FlagsRegister: CF=" << carryFlag << ", ZF=" << zeroFlag << std::endl;
}

void Core::FlagsRegister::reset() {
    carryFlag = false;
    zeroFlag = false;

    notifyObserver();
}

void Core::FlagsRegister::in() {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister: in - will read from ALU on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Core::FlagsRegister::readFromAlu() {
    bool aluCarry = arithmeticLogicUnit->isCarry();
    bool aluZero = arithmeticLogicUnit->isZero();

    if (Utils::debugL2()) {
        std::cout << "FlagsRegister: read from ALU. Changing values from CF=" << carryFlag << ", ZF=" << zeroFlag
                  << " to CF=" << aluCarry << ", ZF=" << aluZero << std::endl;
    }

    carryFlag = aluCarry;
    zeroFlag = aluZero;

    notifyObserver();
}

void Core::FlagsRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << "FlagsRegister: clock ticked" << std::endl;
    }
    
    if (readOnClock) {
        readFromAlu();
        readOnClock = false;
    }
}

bool Core::FlagsRegister::isCarryFlag() const {
    return carryFlag;
}

bool Core::FlagsRegister::isZeroFlag() const {
    return zeroFlag;
}

void Core::FlagsRegister::notifyObserver() const {
    if (observer != nullptr) {
        observer->flagsUpdated(carryFlag, zeroFlag);
    }
}

void Core::FlagsRegister::setObserver(const std::shared_ptr<FlagsRegisterObserver> &newObserver) {
    observer = newObserver;
}
