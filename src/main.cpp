#include <iostream>
#include <memory>

#include "ui/UserInterface.h"

int main(int argc, char **argv) {
    std::cout << "Starting the 8-bit-computer emulator" << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: 8bit <program.asm>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string fileName = argv[1];

    try {
        const auto ui = std::make_unique<UI::UserInterface>(fileName);
        ui->start();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Finished" << std::endl;

    return EXIT_SUCCESS;
}
