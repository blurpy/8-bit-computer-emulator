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
    ram = std::make_shared<RandomAccessMemory>(bus);
    mar = std::make_unique<MemoryAddressRegister>(ram, bus);
    pc = std::make_unique<ProgramCounter>(bus);
    instructionRegister = std::make_unique<InstructionRegister>(bus);
}

Emulator::~Emulator() {
    std::cout << "Emulator out" << std::endl;
}

void Emulator::run() {
    std::cout << "Emulator: run start" << std::endl;

//    clock->setFrequency(1);
//    clock->addListener(this);
//    clock->start();

    reset();
    printValues();

    std::cout << "Emulator: program memory" << std::endl;
    mar->program(0);
    ram->program(10);
    mar->program(1);
    ram->program(11);

    reset();

    std::cout << "Emulator: step 1 - fetch" << std::endl;
    pc->writeToBus(); // CO
    mar->readFromBus(); // MI

    std::cout << "Emulator: step 2 - fetch" << std::endl;
    ram->writeToBus(); // RO
    instructionRegister->readFromBus(); // II
    pc->increment(); // CE

//    this->bus->write(28);
//    this->aRegister->readFromBus();
//    this->bus->write(14);
//    this->bRegister->readFromBus();
//    this->alu->writeToBus();
//    this->aRegister->readFromBus();

    printValues();

//    std::this_thread::sleep_for(std::chrono::milliseconds(3100));
//    clock->stop();
//    clock->singleStep();

    std::cout << "Emulator: run stop" << std::endl;
}

void Emulator::printValues() {
    std::cout << "Emulator: print current values" << std::endl;

    bus->print();
    aRegister->print();
    bRegister->print();
    alu->print();
    mar->print();
    ram->print();
    pc->print();
    instructionRegister->print();
}

void Emulator::reset() {
    std::cout << "Emulator: reset" << std::endl;

    bus->reset();
    aRegister->reset();
    bRegister->reset();
    alu->reset();
    mar->reset();
    ram->reset();
    pc->reset();
    instructionRegister->reset();
}

void ClockListener::clockTicked() {
    std::cout << "Emulator: clock ticked" << std::endl;
}

void ClockListener::invertedClockTicked() {
    std::cout << "Emulator: inverted clock ticked" << std::endl;
}
