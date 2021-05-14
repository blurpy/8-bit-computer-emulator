#include <iostream>
#include <thread>

#include "Window.h"

#include "../core/Utils.h"

#include "UserInterface.h"

UI::UserInterface::UserInterface() {
    if (Core::Utils::debugL2()) {
        std::cout << "UserInterface construct" << std::endl;
    }

    this->running = false;
    this->window = std::make_unique<Window>("8bit");
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

    while (running) {
        window->clearScreen();
        window->drawText("Hello 8-bit!", 5, 0);
        window->redraw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
