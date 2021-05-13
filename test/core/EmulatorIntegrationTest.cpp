#include <doctest.h>

#include "core/Emulator.h"

using namespace Core;

TEST_SUITE("EmulatorIntegrationTest") {
    TEST_CASE("emulator should work correctly") {
        Emulator emulator;

        emulator.setFrequency(2000);

        SUBCASE("run() should throw exception if file does not exist") {
            CHECK_THROWS_WITH(emulator.run("../../programs/test/does_not_exist.asm"),
                              "Assembler: failed to open file: ../../programs/test/does_not_exist.asm");
        }

        SUBCASE("run() should throw exception if file is empty") {
            CHECK_THROWS_WITH(emulator.run("../../programs/test/empty_test.asm"),
                              "Emulator: no instructions loaded. Aborting");
        }

        SUBCASE("run() should complete nop_test.asm") {
            emulator.run("../../programs/nop_test.asm");
        }

        SUBCASE("run() should complete add_two_numbers.asm") {
            emulator.run("../../programs/add_two_numbers.asm");
        }

        SUBCASE("run() should complete subtract_two_numbers.asm") {
            emulator.run("../../programs/subtract_two_numbers.asm");
        }

        SUBCASE("run() should complete multiply_two_numbers.asm") {
            emulator.run("../../programs/multiply_two_numbers.asm");
        }

        SUBCASE("run() should complete count_0_255_stop.asm") {
            emulator.run("../../programs/count_0_255_stop.asm");
        }

        SUBCASE("run() should complete count_255_0_stop.asm") {
            emulator.run("../../programs/count_255_0_stop.asm");
        }
    }
}
