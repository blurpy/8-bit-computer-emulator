#include <iostream>
#include <thread>

#include "Assembler.h"
#include "Utils.h"

#include "Emulator.h"

Emulator::Emulator() {
    if (Utils::debugL2()) {
        std::cout << "Emulator in" << std::endl;
    }

    clock = std::make_shared<Clock>();
    bus = std::make_shared<Bus>();
    aRegister = std::make_shared<GenericRegister>("A", bus);
    bRegister = std::make_shared<GenericRegister>("B", bus);
    arithmeticLogicUnit = std::make_shared<ArithmeticLogicUnit>(aRegister, bRegister, bus);
    randomAccessMemory = std::make_shared<RandomAccessMemory>(bus);
    memoryAddressRegister = std::make_shared<MemoryAddressRegister>(randomAccessMemory, bus);
    programCounter = std::make_shared<ProgramCounter>(bus);
    instructionRegister = std::make_shared<InstructionRegister>(bus);
    outputRegister = std::make_shared<OutputRegister>(bus);
    flagsRegister = std::make_shared<FlagsRegister>(arithmeticLogicUnit);
    instructionDecoder = std::make_shared<InstructionDecoder>(memoryAddressRegister, programCounter,
                                                              randomAccessMemory, instructionRegister, aRegister,
                                                              bRegister, arithmeticLogicUnit, outputRegister,
                                                              flagsRegister, clock);
    stepCounter = std::make_unique<StepCounter>(instructionDecoder);

    // Cyclic dependency - also, setting it here to reuse the shared pointers
    aRegister->setRegisterListener(arithmeticLogicUnit);
    bRegister->setRegisterListener(arithmeticLogicUnit);

    // This order is also the order the components receive ticks from the clock.
    // It's important that flags are read first since SUB and ADD change the value in registers on the same clock tick.
    clock->addListener(flagsRegister.get());
    clock->addListener(memoryAddressRegister.get());
    clock->addListener(stepCounter.get());
    clock->addListener(instructionRegister.get());
    clock->addListener(programCounter.get());
    clock->addListener(aRegister.get());
    clock->addListener(bRegister.get());
    clock->addListener(outputRegister.get());
    clock->addListener(randomAccessMemory.get());
}

Emulator::~Emulator() {
    if (Utils::debugL2()) {
        std::cout << "Emulator out" << std::endl;
    }

    // Fix memory not being freed automatically, probably due to cyclic reference
    aRegister->setRegisterListener(nullptr);
    bRegister->setRegisterListener(nullptr);
}

void Emulator::run(const std::string &fileName) {
    if (Utils::debugL1()) {
        std::cout << "Emulator: run start" << std::endl;
    }

    reset();

    if (!programMemory(fileName)) {
        std::cerr << "Emulator: no instructions loaded. Aborting" << std::endl;
        return;
    }

    if (Utils::debugL1()) {
        std::cout << "Emulator: run clock" << std::endl;
    }

    clock->setFrequency(20);
    clock->start();
    clock->join();

    printValues();

    if (Utils::debugL1()) {
        std::cout << "Emulator: run stop" << std::endl;
    }
}

bool Emulator::programMemory(const std::string &fileName) {
    std::cout << "Emulator: program memory" << std::endl;

    auto assembler = std::make_unique<Assembler>();
    const std::vector<Assembler::Instruction> instructions = assembler->loadInstructions(fileName);

    if (instructions.empty()) {
        return false;
    }

    for (auto instruction : instructions) {
        memoryAddressRegister->program(instruction.address);
        randomAccessMemory->program(instruction.opcode, instruction.operand);
    }

    return true;
}

void Emulator::printValues() {
    std::cout << "Emulator: print current values" << std::endl;

    bus->print();
    aRegister->print();
    bRegister->print();
    arithmeticLogicUnit->print();
    memoryAddressRegister->print();
    randomAccessMemory->print();
    programCounter->print();
    instructionRegister->print();
    outputRegister->print();
    stepCounter->print();
    flagsRegister->print();
}

void Emulator::reset() {
    std::cout << "Emulator: reset" << std::endl;

    bus->reset();
    aRegister->reset();
    bRegister->reset();
    arithmeticLogicUnit->reset();
    memoryAddressRegister->reset();
    randomAccessMemory->reset();
    programCounter->reset();
    instructionRegister->reset();
    outputRegister->reset();
    stepCounter->reset();
    flagsRegister->reset();
}
