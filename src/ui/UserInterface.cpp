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
    this->keyboard = std::make_shared<Keyboard>(this->emulator);
    this->window = std::make_unique<Window>("8bit " + fileName, this->keyboard);

    this->clock = std::make_shared<ClockModel>();
    this->bus = std::make_shared<ValueModel>("Bus", 8);
    this->aRegister = std::make_shared<ValueModel>("A Register", 8);
    this->bRegister = std::make_shared<ValueModel>("B Register", 8);
    this->arithmeticLogicUnit = std::make_shared<ArithmeticLogicUnitModel>();
    this->memoryAddressRegister = std::make_shared<ValueModel>("Memory Address Register", 4);
    this->programCounter = std::make_shared<ValueModel>("Program Counter", 4);
    this->randomAccessMemory = std::make_shared<RandomAccessMemoryModel>(this->memoryAddressRegister);
    this->instructionRegister = std::make_shared<ValueModel>("Instruction Register", 8);
    this->outputRegister = std::make_shared<ValueModel>("Output Register", 8);
    this->stepCounter = std::make_shared<ValueModel>("Step Counter", 3);
    this->flagsRegister = std::make_shared<FlagsRegisterModel>();
    this->instruction = std::make_unique<InstructionModel>(this->stepCounter, this->instructionRegister);
    this->instructionDecoder = std::make_shared<InstructionDecoderModel>();

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
    this->emulator->setInstructionDecoderObserver(this->instructionDecoder);
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

    try {
        while (!window->isClosed()) {
            window->pollEvents();
        }

        running = false;
        mainThread.join();
    }

    // Stop the thread controlled, otherwise it will crash hard with no message
    catch (const std::runtime_error &e) {
        running = false;
        mainThread.join();

        throw e;
    }
}

void UI::UserInterface::mainLoop() {
    std::cout << std::endl << "UserInterface: starting main loop" << std::endl << std::endl;

    emulator->load(fileName);
    emulator->setFrequency(2);

    while (running) {
        window->clearScreen();

        drawLeftColumn();
        drawRightColumn();

        window->redraw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    emulator->stop();
}

void UI::UserInterface::drawLeftColumn() {
    int currentLine = 0;

    drawLeftText(clock->getRenderText(), currentLine++);
    drawLeftText(programCounter->getRenderText(), currentLine++);
    drawLeftText(bus->getRenderText(), currentLine++);
    drawLeftText(aRegister->getRenderText(), currentLine++);
    drawLeftText(bRegister->getRenderText(), currentLine++);
    drawLeftText(arithmeticLogicUnit->getRenderText(), currentLine++);
    drawLeftText(flagsRegister->getRenderText(), currentLine++);
    drawLeftText(memoryAddressRegister->getRenderText(), currentLine++);
    drawLeftText(randomAccessMemory->getRenderText(), currentLine++);
    drawLeftText(stepCounter->getRenderText(), currentLine++);
    drawLeftText(instructionRegister->getRenderText(), currentLine++);
    drawLeftText(instruction->getRenderText(), currentLine++);
    drawLeftText(outputRegister->getRenderText(), currentLine++);

    currentLine++;

    drawLeftText(instructionDecoder->getRenderValueText(), currentLine++);
    drawLeftText(instructionDecoder->getRenderTitleText(), currentLine);
}

void UI::UserInterface::drawLeftText(const std::string &text, const int currentLine) {
    window->drawText(text, LEFT_POSITION, currentLine * LINE_HEIGHT);
}

void UI::UserInterface::drawRightColumn() {
    const std::array<std::string, RandomAccessMemoryModel::MEMORY_SIZE> &ramValues = randomAccessMemory->getRenderTextFull();

    for (int i = 0; i < ramValues.size(); i++) {
        const std::string &ramValue = ramValues[i];

        if (memoryAddressRegister->getValue() == i) {
            window->drawText("* " + ramValue, RIGHT_MARKER_POSITION, i * LINE_HEIGHT);
        } else {
            window->drawText(ramValue, RIGHT_POSITION, i * LINE_HEIGHT);
        }
    }
}
