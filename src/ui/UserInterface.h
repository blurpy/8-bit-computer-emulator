#ifndef INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H
#define INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H

#include <memory>

#include "ValueModel.h"
#include "Window.h"

#include "../core/Emulator.h"

namespace UI {

    class UserInterface {

    public:
        explicit UserInterface(const std::string &fileName);
        ~UserInterface();

        void start();

    private:
        std::unique_ptr<Window> window;
        std::shared_ptr<Core::Emulator> emulator;
        std::shared_ptr<ValueModel> aRegister;
        std::shared_ptr<ValueModel> bRegister;
        std::shared_ptr<ValueModel> outputRegister;

        std::string fileName;
        bool running;

        void mainLoop();
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H
