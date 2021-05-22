#include <iostream>

#include "Utils.h"

#include "GenericRegister.h"

Core::GenericRegister::GenericRegister(const std::string& name, const std::shared_ptr<Bus> &bus) {
    this->name = name;
    this->bus = bus;
    this->value = 0;
    this->readOnClock = false;

    if (Utils::debugL2()) {
        std::cout << this->name << " register construct" << std::endl;
    }
}

Core::GenericRegister::~GenericRegister() {
    if (Utils::debugL2()) {
        std::cout << this->name << " register destruct" << std::endl;
    }
}

void Core::GenericRegister::readFromBus() {
    uint8_t busValue = bus->read();

    if (Utils::debugL2()) {
        std::cout << name << " register: changing value from " << (int) value << " to " << (int) busValue << std::endl;
    }

    value = busValue;

    notifyObserver();
    notifyListener();
}

void Core::GenericRegister::writeToBus() {
    bus->write(value);
}

uint8_t Core::GenericRegister::readValue() const {
    return value;
}

void Core::GenericRegister::print() {
    printf("%s register: %d / 0x%02X / " BYTE_PATTERN " \n", name.c_str(), value, value, BYTE_TO_BINARY(value));
}

void Core::GenericRegister::reset() {
    value = 0;

    notifyObserver();
    notifyListener();
}

void Core::GenericRegister::in() {
    if (Utils::debugL2()) {
        std::cout << name << " register: in - will read from bus on clock tick" << std::endl;
    }

    readOnClock = true;
}

void Core::GenericRegister::out() {
    if (Utils::debugL2()) {
        std::cout << name << " register: out" << std::endl;
    }

    writeToBus();
}

void Core::GenericRegister::clockTicked() {
    if (Utils::debugL2()) {
        std::cout << name << " register: clock ticked" << std::endl;
    }

    if (readOnClock) {
        readFromBus();
        readOnClock = false;
    }
}

void Core::GenericRegister::notifyObserver() const {
    if (observer != nullptr) {
        observer->valueUpdated(value);
    }
}

void Core::GenericRegister::notifyListener() const {
    if (registerListener != nullptr) {
        registerListener->registerValueChanged(value);
    }
}

void Core::GenericRegister::setRegisterListener(const std::shared_ptr<RegisterListener> &newRegisterListener) {
    registerListener = newRegisterListener;
}

void Core::GenericRegister::setObserver(const std::shared_ptr<ValueObserver> &newObserver) {
    observer = newObserver;
}
