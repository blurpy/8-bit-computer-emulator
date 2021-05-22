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
        emulator.setFrequency(5000);

        SUBCASE("load() should throw exception if file does not exist") {
            CHECK_THROWS_WITH(emulator.load("../../programs/test/does_not_exist.asm"),
                              "Assembler: failed to open file: ../../programs/test/does_not_exist.asm");
        }

        SUBCASE("load() should throw exception if file is empty") {
            CHECK_THROWS_WITH(emulator.load("../../programs/test/empty_test.asm"),
                              "Emulator: no instructions loaded. Aborting");
        }

        SUBCASE("startSynchronous() should complete nop_test.asm") {
            emulator.load("../../programs/nop_test.asm");

            emulator.startSynchronous();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(10)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("startSynchronous() should complete add_two_numbers.asm") {
            emulator.load("../../programs/add_two_numbers.asm");

            emulator.startSynchronous();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(42)).Once(); // 28+14
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("startSynchronous() should complete subtract_two_numbers.asm") {
            emulator.load("../../programs/subtract_two_numbers.asm");

            emulator.startSynchronous();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(18)).Once(); // 30-12
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("startSynchronous() should complete multiply_two_numbers.asm") {
            emulator.load("../../programs/multiply_two_numbers.asm");

            emulator.startSynchronous();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once(); // Reset before start
            fakeit::Verify(Method(observerMock, valueUpdated).Using(56)).Once(); // 7*8
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("startSynchronous() should complete count_0_255_stop.asm") {
            emulator.load("../../programs/count_0_255_stop.asm");

            emulator.startSynchronous();

            // Once for reset, and once when counting
            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Twice();

            for (int i = 1; i <= 255; i++) {
                fakeit::Verify(Method(observerMock, valueUpdated).Using(i)).Once();
            }

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("startSynchronous() should complete count_255_0_stop.asm") {
            emulator.load("../../programs/count_255_0_stop.asm");

            emulator.startSynchronous();

            // Once for reset, and once when counting
            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Twice();

            for (int i = 1; i <= 255; i++) {
                fakeit::Verify(Method(observerMock, valueUpdated).Using(i)).Once();
            }

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("reload() should reset all state including memory") {
            emulator.load("../../programs/memory_test.asm");

            emulator.startSynchronous();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once();
            fakeit::Verify(Method(observerMock, valueUpdated).Using(7)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);

            observerMock.ClearInvocationHistory();

            emulator.reload();
            emulator.startSynchronous();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once();
            // This would be 11 if memory wasn't reset
            fakeit::Verify(Method(observerMock, valueUpdated).Using(7)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("increaseFrequency() and decreaseFrequency() should work") {
            fakeit::Mock<ClockObserver> clockObserver;
            auto clockPtr = std::shared_ptr<ClockObserver>(&clockObserver(), [](...) {});
            emulator.setClockObserver(clockPtr);

            fakeit::When(Method(clockObserver, frequencyChanged)).AlwaysReturn();

            emulator.setFrequency(2000);
            clockObserver.ClearInvocationHistory();

            emulator.increaseFrequency();
            fakeit::Verify(Method(clockObserver, frequencyChanged).Using(3000)).Once();

            emulator.decreaseFrequency();
            fakeit::Verify(Method(clockObserver, frequencyChanged).Using(2000)).Once();
        }

        SUBCASE("startAsynchronous() and stop() should work") {
            CHECK_FALSE(emulator.isRunning());

            emulator.load("../../programs/count_255_0_stop.asm");
            emulator.setFrequency(10);

            emulator.startAsynchronous();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            CHECK(emulator.isRunning());

            emulator.stop();

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            CHECK_FALSE(emulator.isRunning());
        }
    }
}
