#include <iostream>
#include <thread>

#include "Emulator.h"

Emulator::Emulator() {
    std::cout << "Emulator in" << std::endl;

    clock = std::make_shared<Clock>();
    bus = std::make_shared<Bus>();
    aRegister = std::make_shared<Register>("A", bus);
    bRegister = std::make_shared<Register>("B", bus);
    alu = std::make_shared<ArithmeticLogicUnit>(aRegister, bRegister, bus);
    ram = std::make_shared<RandomAccessMemory>(bus);
    mar = std::make_shared<MemoryAddressRegister>(ram, bus);
    pc = std::make_shared<ProgramCounter>(bus);
    instructionRegister = std::make_shared<InstructionRegister>(bus);
    out = std::make_shared<OutputRegister>(bus);
    stepCounter = std::make_unique<StepCounter>();
    microcode = std::make_shared<Microcode>(mar, pc, ram, instructionRegister, aRegister, bRegister, alu, out, clock);

    stepCounter->setStepListener(microcode);

    clock->addListener(mar.get());
    clock->addListener(stepCounter.get());
    clock->addListener(instructionRegister.get());
    clock->addListener(pc.get());
    clock->addListener(aRegister.get());
    clock->addListener(bRegister.get());
    clock->addListener(out.get());
}

Emulator::~Emulator() {
    std::cout << "Emulator out" << std::endl;
}

void Emulator::run() {
    std::cout << "Emulator: run start" << std::endl;

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

    std::cout << "Emulator: run clock" << std::endl;

    clock->setFrequency(20);
    clock->start();
    clock->join();

    printValues();

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
    stepCounter->print();
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
    stepCounter->reset();
}
