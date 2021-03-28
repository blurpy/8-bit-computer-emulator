#include <iostream>
#include <thread>

#include "Emulator.h"

Emulator::Emulator() {
    std::cout << "Emulator in" << std::endl;

    clock = std::make_shared<Clock>();
    bus = std::make_shared<Bus>();
    aRegister = std::make_shared<Register>("A", bus);
    bRegister = std::make_shared<Register>("B", bus);
    alu = std::make_unique<ArithmeticLogicUnit>(aRegister, bRegister, bus);
}

Emulator::~Emulator() {
    std::cout << "Emulator out" << std::endl;
}

void Emulator::run() {
    clock->setFrequency(0.5);
    clock->addListener(this);
    clock->start();

    printValues();

    this->bus->write(28);
    this->aRegister->readFromBus();
    this->bus->write(14);
    this->bRegister->readFromBus();
    this->alu->writeToBus();
    this->aRegister->readFromBus();

    printValues();

    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    clock->stop();
}

void Emulator::printValues() {
    std::cout << "Emulator: print current values" << std::endl;
    bus->print();
    aRegister->print();
    bRegister->print();
    alu->print();
}

void ClockListener::clockTicked() {
    std::cout << "Emulator: clock ticked" << std::endl;
}

void ClockListener::invertedClockTicked() {
    std::cout << "Emulator: inverted clock ticked" << std::endl;
}
