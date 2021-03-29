#include <cassert>
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
    out = std::make_unique<OutputRegister>(bus);
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
    // LDA 14
    mar->program(std::bitset<4>("0000"));
    ram->program(std::bitset<4>("0001"), std::bitset<4>("1110"));
    // ADD 15
    mar->program(std::bitset<4>("0001"));
    ram->program(std::bitset<4>("0010"), std::bitset<4>("1111"));
    // OUT
    mar->program(std::bitset<4>("0010"));
    ram->program(std::bitset<4>("1110"), std::bitset<4>("0000"));
    // HLT
    mar->program(std::bitset<4>("0011"));
    ram->program(std::bitset<4>("1111"), std::bitset<4>("0000"));
    // X = 28
    mar->program(std::bitset<4>("1110"));
    ram->program(std::bitset<8>("00011100"));
    // Y = 14
    mar->program(std::bitset<4>("1111"));
    ram->program(std::bitset<8>("00001110"));

    printValues();

    // LDA 14 - Put value at memory address 14 (=28) in A register

    std::cout << "Emulator: LDA step 1 - fetch" << std::endl;
    pc->writeToBus(); // CO
    mar->readFromBus(); // MI

    std::cout << "Emulator: LDA step 2 - fetch" << std::endl;
    ram->writeToBus(); // RO
    instructionRegister->readFromBus(); // II
    pc->increment(); // CE

    std::cout << "Emulator: LDA step 3 - lda" << std::endl;
    instructionRegister->writeToBus(); // IO
    mar->readFromBus(); // MI

    std::cout << "Emulator: LDA step 4 - lda" << std::endl;
    ram->writeToBus(); // RO
    aRegister->readFromBus(); // AI

    printValues();
    assert(aRegister->readValue() == 28);

    // ADD 15 - Put value at memory address 15 (=14) in B register and add A+B into A

    std::cout << "Emulator: ADD step 1 - fetch" << std::endl;
    pc->writeToBus(); // CO
    mar->readFromBus(); // MI

    std::cout << "Emulator: ADD step 2 - fetch" << std::endl;
    ram->writeToBus(); // RO
    instructionRegister->readFromBus(); // II
    pc->increment(); // CE

    std::cout << "Emulator: ADD step 3 - add" << std::endl;
    instructionRegister->writeToBus(); // IO
    mar->readFromBus(); // MI

    std::cout << "Emulator: ADD step 4 - add" << std::endl;
    ram->writeToBus(); // RO
    bRegister->readFromBus(); // BI

    std::cout << "Emulator: ADD step 5 - add" << std::endl;
    alu->writeToBus(); // SO
    aRegister->readFromBus(); // AI

    printValues();
    assert(bRegister->readValue() == 14);
    assert(aRegister->readValue() == 42);

    // OUT - Print result on output display

    std::cout << "Emulator: OUT step 1 - fetch" << std::endl;
    pc->writeToBus(); // CO
    mar->readFromBus(); // MI

    std::cout << "Emulator: OUT step 2 - fetch" << std::endl;
    ram->writeToBus(); // RO
    instructionRegister->readFromBus(); // II
    pc->increment(); // CE

    std::cout << "Emulator: OUT step 3 - out" << std::endl;
    aRegister->writeToBus(); // AO
    out->readFromBus(); // OI

    printValues();
    assert(out->readValue() == 42);

//    this->bus->write(28);
//    this->aRegister->readFromBus();
//    this->bus->write(14);
//    this->bRegister->readFromBus();
//    this->alu->writeToBus();
//    this->aRegister->readFromBus();

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
    out->print();
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
    out->reset();
}

void ClockListener::clockTicked() {
    std::cout << "Emulator: clock ticked" << std::endl;
}

void ClockListener::invertedClockTicked() {
    std::cout << "Emulator: inverted clock ticked" << std::endl;
}
