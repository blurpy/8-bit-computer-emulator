#ifndef INC_8_BIT_COMPUTER_EMULATOR_KEYBOARD_H
#define INC_8_BIT_COMPUTER_EMULATOR_KEYBOARD_H

#include <memory>
#include <SDL.h>

#include "../core/Emulator.h"

namespace UI {

    /**
     * Handles keyboard events for controlling the emulator.
     */
    class Keyboard {

    public:
        explicit Keyboard(const std::shared_ptr<Core::Emulator> &emulator);
        ~Keyboard();

        void keyUp(SDL_Keycode keycode);

    private:
        std::shared_ptr<Core::Emulator> emulator;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_KEYBOARD_H
