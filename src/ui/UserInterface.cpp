#include <iostream>
#include <thread>

#include "Window.h"

#include "../core/Utils.h"

#include "UserInterface.h"

UI::UserInterface::UserInterface(const std::string &fileName) {
    if (Core::Utils::debugL2()) {
        std::cout << "UserInterface construct" << std::endl;
    }

    this->fileName = fileName;
    this->running = false;

    this->emulator = std::make_shared<Core::Emulator>();
    this->window = std::make_unique<Window>("8bit");

    this->clock = std::make_shared<ClockModel>();
    this->bus = std::make_shared<ValueModel>("Bus", 8);
    this->aRegister = std::make_shared<ValueModel>("A Register", 8);
    this->bRegister = std::make_shared<ValueModel>("B Register", 8);
    this->arithmeticLogicUnit = std::make_shared<ArithmeticLogicUnitModel>();
    this->memoryAddressRegister = std::make_shared<ValueModel>("Memory Address Register", 4);
    this->programCounter = std::make_shared<ValueModel>("Program Counter", 4);
    this->randomAccessMemory = std::make_shared<ValueModel>("Random Access Memory", 8);
    this->instructionRegister = std::make_shared<ValueModel>("Instruction Register", 8);
    this->outputRegister = std::make_shared<ValueModel>("Output Register", 8);
    this->stepCounter = std::make_shared<ValueModel>("Step Counter", 3);
    this->flagsRegister = std::make_shared<FlagsRegisterModel>();
    this->instruction = std::make_unique<InstructionModel>(this->stepCounter, this->instructionRegister);

    this->emulator->setClockObserver(this->clock);
    this->emulator->setBusObserver(this->bus);
    this->emulator->setARegisterObserver(this->aRegister);
    this->emulator->setBRegisterObserver(this->bRegister);
    this->emulator->setArithmeticLogicUnitObserver(this->arithmeticLogicUnit);
    this->emulator->setMemoryAddressRegisterObserver(this->memoryAddressRegister);
    this->emulator->setProgramCounterObserver(this->programCounter);
    this->emulator->setRandomAccessMemoryObserver(this->randomAccessMemory);
    this->emulator->setInstructionRegisterObserver(this->instructionRegister);
    this->emulator->setOutputRegisterObserver(this->outputRegister);
    this->emulator->setStepCounterObserver(this->stepCounter);
    this->emulator->setFlagsRegisterObserver(this->flagsRegister);
}

UI::UserInterface::~UserInterface() {
    if (Core::Utils::debugL2()) {
        std::cout << "UserInterface destruct" << std::endl;
    }
}

void UI::UserInterface::start() {
    if (!window->show()) {
        throw std::runtime_error("UserInterface: failed to open window");
    }

    running = true;
    std::thread mainThread(&UserInterface::mainLoop, this);

    while (!window->isClosed()) {
        window->pollEvents();
    }

    running = false;
    mainThread.join();
}

void UI::UserInterface::mainLoop() {
    std::cout << std::endl << "UserInterface: starting main loop" << std::endl << std::endl;

    emulator->setFrequency(5);
    emulator->run(fileName);

    while (running) {
        int currentLine = 0;

        window->clearScreen();

        drawText(clock->getRenderText(), currentLine++);
        drawText(programCounter->getRenderText(), currentLine++);
        drawText(bus->getRenderText(), currentLine++);
        drawText(aRegister->getRenderText(), currentLine++);
        drawText(bRegister->getRenderText(), currentLine++);
        drawText(arithmeticLogicUnit->getRenderText(), currentLine++);
        drawText(flagsRegister->getRenderText(), currentLine++);
        drawText(memoryAddressRegister->getRenderText(), currentLine++);
        drawText(randomAccessMemory->getRenderText(), currentLine++);
        drawText(stepCounter->getRenderText(), currentLine++);
        drawText(instructionRegister->getRenderText(), currentLine++);
        drawText(instruction->getRenderText(), currentLine++);
        drawText(outputRegister->getRenderText(), currentLine);

        window->redraw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    emulator->stop();
    emulator->waitUntilFinished();
}

void UI::UserInterface::drawText(const std::string &text, const int currentLine) {
    window->drawText(text, LEFT_POSITION, currentLine * LINE_HEIGHT);
}
