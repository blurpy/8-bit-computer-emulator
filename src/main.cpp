#include <iostream>
#include <memory>

#include "Emulator.h"

int main(int argc, char **argv) {
    std::cout << "Hello!" << std::endl;

    const auto emulator = std::make_unique<Emulator>();
    emulator->run();

    std::cout << "Good bye!" << std::endl;

    return EXIT_SUCCESS;
}
