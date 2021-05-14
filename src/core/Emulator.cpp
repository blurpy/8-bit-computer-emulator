#include <iostream>
#include <thread>

#include "Assembler.h"
#include "Utils.h"

#include "Emulator.h"

Core::Emulator::Emulator() {
    if (Utils::debugL2()) {
        std::cout << "Emulator construct" << std::endl;
    }

    timeSource = std::make_shared<TimeSource>();
    clock = std::make_shared<Clock>(timeSource);
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
    stepCounter = std::make_shared<StepCounter>(instructionDecoder);

    // Cyclic dependency - also, setting it here to reuse the shared pointers
    aRegister->setRegisterListener(arithmeticLogicUnit);
    bRegister->setRegisterListener(arithmeticLogicUnit);

    // This order is also the order the components receive ticks from the clock.
    // It's important that flags are read first since SUB and ADD change the value in registers on the same clock tick.
    clock->addListener(flagsRegister);
    clock->addListener(memoryAddressRegister);
    clock->addListener(stepCounter);
    clock->addListener(instructionRegister);
    clock->addListener(programCounter);
    clock->addListener(aRegister);
    clock->addListener(bRegister);
    clock->addListener(outputRegister);
    clock->addListener(randomAccessMemory);
}

Core::Emulator::~Emulator() {
    if (Utils::debugL2()) {
        std::cout << "Emulator destruct" << std::endl;
    }

    // Fix memory not being freed automatically, probably due to cyclic reference
    aRegister->setRegisterListener(nullptr);
    bRegister->setRegisterListener(nullptr);

    clock->clearListeners();
}

void Core::Emulator::run(const std::string &fileName) {
    if (Utils::debugL1()) {
        std::cout << "Emulator: run start" << std::endl;
    }

    reset();

    if (!programMemory(fileName)) {
        throw std::runtime_error("Emulator: no instructions loaded. Aborting");
    }

    if (Utils::debugL1()) {
        std::cout << "Emulator: run clock" << std::endl;
    }

    clock->start();

    printValues();

    if (Utils::debugL1()) {
        std::cout << "Emulator: run stop" << std::endl;
    }
}

void Core::Emulator::waitUntilFinished() {
    clock->join();
}

void Core::Emulator::stop() {
    clock->stop();
}

void Core::Emulator::setFrequency(double hz) {
    clock->setFrequency(hz);
}

bool Core::Emulator::programMemory(const std::string &fileName) {
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

void Core::Emulator::printValues() {
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

void Core::Emulator::reset() {
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

void Core::Emulator::setBusObserver(const std::shared_ptr<ValueObserver> &observer) {
    bus->setObserver(observer);
}

void Core::Emulator::setARegisterObserver(const std::shared_ptr<ValueObserver> &observer) {
    aRegister->setObserver(observer);
}

void Core::Emulator::setBRegisterObserver(const std::shared_ptr<ValueObserver> &observer) {
    bRegister->setObserver(observer);
}

void Core::Emulator::setMemoryAddressRegisterObserver(const std::shared_ptr<ValueObserver> &observer) {
    memoryAddressRegister->setObserver(observer);
}

void Core::Emulator::setInstructionRegisterObserver(const std::shared_ptr<ValueObserver> &observer) {
    instructionRegister->setObserver(observer);
}

void Core::Emulator::setOutputRegisterObserver(const std::shared_ptr<ValueObserver> &observer) {
    outputRegister->setObserver(observer);
}

void Core::Emulator::setProgramCounterObserver(const std::shared_ptr<ValueObserver> &observer) {
    programCounter->setObserver(observer);
}

void Core::Emulator::setStepCounterObserver(const std::shared_ptr<ValueObserver> &observer) {
    stepCounter->setObserver(observer);
}
