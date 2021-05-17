#include <iostream>

#include "../core/Utils.h"

#include "Keyboard.h"

UI::Keyboard::Keyboard(const std::shared_ptr<Core::Emulator> &emulator) {
    if (Core::Utils::debugL2()) {
        std::cout << "Keyboard construct" << std::endl;
    }

    this->emulator = emulator;
}

UI::Keyboard::~Keyboard() {
    if (Core::Utils::debugL2()) {
        std::cout << "Keyboard destruct" << std::endl;
    }
}

void UI::Keyboard::keyUp(const SDL_Keycode keycode) {
    // s: start / stop
    if (keycode == SDLK_s) {
        if (emulator->isRunning()) {
            emulator->stop();
        } else {
            emulator->startAsynchronous();
        }
    }

    // r: reload ram and reset everything else
    else if (keycode == SDLK_r) {
        if (!emulator->isRunning()) {
            emulator->reload();
        }
    }

    // space: single step
    else if (keycode == SDLK_SPACE) {
        emulator->singleStep();
    }

    // +: increase frequency
    else if (keycode == SDLK_PLUS || keycode == SDLK_KP_PLUS) {
        emulator->increaseFrequency();
    }

    // -: decrease frequency
    else if (keycode == SDLK_MINUS || keycode == SDLK_KP_MINUS) {
        emulator->decreaseFrequency();
    }
}
