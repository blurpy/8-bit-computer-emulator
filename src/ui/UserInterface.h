#ifndef INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H
#define INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H

#include <memory>

#include "ArithmeticLogicUnitModel.h"
#include "ClockModel.h"
#include "FlagsRegisterModel.h"
#include "InstructionDecoderModel.h"
#include "InstructionModel.h"
#include "RandomAccessMemoryModel.h"
#include "ValueModel.h"
#include "Window.h"

#include "../core/Emulator.h"

namespace UI {

    /**
     * Manages the user interface, as well as the emulator behind.
     */
    class UserInterface {

    public:
        explicit UserInterface(const std::string &fileName);
        ~UserInterface();

        void start();

    private:
        static const int LINE_HEIGHT = 24;
        static const int LEFT_POSITION = 5;
        static const int RIGHT_POSITION = 640;
        static const int RIGHT_MARKER_POSITION = 618;

        std::unique_ptr<Window> window;
        std::shared_ptr<Keyboard> keyboard;
        std::shared_ptr<Core::Emulator> emulator;

        std::shared_ptr<ClockModel> clock;
        std::shared_ptr<ValueModel> bus;
        std::shared_ptr<ValueModel> aRegister;
        std::shared_ptr<ValueModel> bRegister;
        std::shared_ptr<ArithmeticLogicUnitModel> arithmeticLogicUnit;
        std::shared_ptr<ValueModel> memoryAddressRegister;
        std::shared_ptr<ValueModel> programCounter;
        std::shared_ptr<RandomAccessMemoryModel> randomAccessMemory;
        std::shared_ptr<ValueModel> instructionRegister;
        std::shared_ptr<ValueModel> outputRegister;
        std::shared_ptr<ValueModel> stepCounter;
        std::shared_ptr<FlagsRegisterModel> flagsRegister;
        std::unique_ptr<InstructionModel> instruction;
        std::shared_ptr<InstructionDecoderModel> instructionDecoder;

        std::string fileName;
        bool running;

        void mainLoop();
        void drawLeftColumn();
        void drawLeftText(const std::string &text, int currentLine);
        void drawRightColumn();
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H
