#include <doctest.h>
#include <fakeit.hpp>

#include "core/Emulator.h"

using namespace Core;

TEST_SUITE("EmulatorIntegrationTest") {
    TEST_CASE("emulator should work correctly") {
        Emulator emulator;

        fakeit::Mock<ValueObserver> observerMock;
        auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});

        fakeit::When(Method(observerMock, valueUpdated)).AlwaysReturn();

        emulator.setOutputRegisterObserver(observerPtr);
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

            emulator.waitUntilFinished();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(10)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("run() should complete add_two_numbers.asm") {
            emulator.run("../../programs/add_two_numbers.asm");

            emulator.waitUntilFinished();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(42)).Once(); // 28+14
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("run() should complete subtract_two_numbers.asm") {
            emulator.run("../../programs/subtract_two_numbers.asm");

            emulator.waitUntilFinished();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(18)).Once(); // 30-12
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("run() should complete multiply_two_numbers.asm") {
            emulator.run("../../programs/multiply_two_numbers.asm");

            emulator.waitUntilFinished();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(56)).Once(); // 7*8
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("run() should complete count_0_255_stop.asm") {
            emulator.run("../../programs/count_0_255_stop.asm");

            emulator.waitUntilFinished();

            // Once for reset, and once when counting
            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Twice();

            for (int i = 1; i <= 255; i++) {
                fakeit::Verify(Method(observerMock, valueUpdated).Using(i)).Once();
            }

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("run() should complete count_255_0_stop.asm") {
            emulator.run("../../programs/count_255_0_stop.asm");

            emulator.waitUntilFinished();

            // Once for reset, and once when counting
            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Twice();

            for (int i = 1; i <= 255; i++) {
                fakeit::Verify(Method(observerMock, valueUpdated).Using(i)).Once();
            }

            fakeit::VerifyNoOtherInvocations(observerMock);
        }
    }
}
