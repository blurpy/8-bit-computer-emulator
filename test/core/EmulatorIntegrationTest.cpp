#include <doctest.h>

#include "core/Emulator.h"

using namespace Core;

TEST_SUITE("EmulatorIntegrationTest") {
    TEST_CASE("run() should throw exception if file does not exist") {
        Emulator emulator;

        CHECK_THROWS_WITH(emulator.run("../../programs/test/does_not_exist.asm"),
                          "Assembler: failed to open file: ../../programs/test/does_not_exist.asm");
    }

    TEST_CASE("run() should throw exception if file is empty") {
        Emulator emulator;

        CHECK_THROWS_WITH(emulator.run("../../programs/test/empty_test.asm"),
                          "Emulator: no instructions loaded. Aborting");
    }

    TEST_CASE("run() should complete nop_test.asm") {
        Emulator emulator;

        emulator.run("../../programs/nop_test.asm");
    }

    TEST_CASE("run() should complete add_two_numbers.asm") {
        Emulator emulator;

        emulator.run("../../programs/add_two_numbers.asm");
    }

    TEST_CASE("run() should complete subtract_two_numbers.asm") {
        Emulator emulator;

        emulator.run("../../programs/subtract_two_numbers.asm");
    }

    TEST_CASE("run() should complete multiply_two_numbers.asm") {
        Emulator emulator;

        emulator.run("../../programs/multiply_two_numbers.asm");
    }

    TEST_CASE("run() should complete count_0_255_stop.asm") {
        Emulator emulator;

        emulator.run("../../programs/count_0_255_stop.asm");
    }

    TEST_CASE("run() should complete count_255_0_stop.asm") {
        Emulator emulator;

        emulator.run("../../programs/count_255_0_stop.asm");
    }
}
