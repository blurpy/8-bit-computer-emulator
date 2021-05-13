#ifndef INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H
#define INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H

#include <memory>

#include "Window.h"

namespace UI {

    class UserInterface {

    public:
        UserInterface();
        ~UserInterface();

        void start();

    private:
        std::unique_ptr<Window> window;
        bool running;

        void mainLoop();
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_USERINTERFACE_H
