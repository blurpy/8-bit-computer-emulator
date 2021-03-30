#include <iostream>
#include <memory>

#include "Emulator.h"

int main(int argc, char **argv) {
    std::cout << "Hello!" << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: emulator <program.asm>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string fileName = argv[1];

    const auto emulator = std::make_unique<Emulator>();
    emulator->run(fileName);

    std::cout << "Good bye!" << std::endl;

    return EXIT_SUCCESS;
}
