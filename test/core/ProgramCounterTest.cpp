#include <doctest.h>
#include <fakeit.hpp>

#include "core/ProgramCounter.h"

using namespace Core;

TEST_SUITE("ProgramCounterTest") {
    TEST_CASE("program counter should work") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();
        ProgramCounter programCounter(bus);

        auto &clock = dynamic_cast<ClockListener&>(programCounter);

        programCounter.out();
        CHECK_EQ(bus->read(), 0);

        SUBCASE("clock tick should do nothing by itself") {
            bus->write(8);

            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(bus->read(), 0); // No jump, and no increment
        }

        SUBCASE("enable() should increment on clock tick") {
            programCounter.enable();

            programCounter.out();
            CHECK_EQ(bus->read(), 0); // Nothing happened yet

            clock.clockTicked();

            CHECK_EQ(bus->read(), 0);
            programCounter.out();
            CHECK_EQ(bus->read(), 1);
        }

        SUBCASE("enable() should only increment on first clock tick") {
            programCounter.enable();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(bus->read(), 1);

            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(bus->read(), 1); // No change
        }

        SUBCASE("enable() should wrap around") {
            for (int i = 0; i < 15; i++) {
                programCounter.enable();
                clock.clockTicked();
            }

            programCounter.out();
            CHECK_EQ(bus->read(), 15);

            programCounter.enable();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("observer should be notified when incrementing") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            programCounter.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).Return();

            programCounter.enable();

            fakeit::VerifyNoOtherInvocations(observerMock); // Nothing happened yet

            clock.clockTicked();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(1)).Once();
        }

        SUBCASE("jump() should change counter to value from bus on clock tick") {
            bus->write(8);

            programCounter.jump();

            programCounter.out();
            CHECK_EQ(bus->read(), 0); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            programCounter.out();
            CHECK_EQ(bus->read(), 6);
        }

        SUBCASE("jump() should only change on first clock tick") {
            bus->write(15);

            programCounter.jump();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(bus->read(), 15);

            bus->write(6);
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(bus->read(), 15); // No change
        }

        SUBCASE("jump() should throw exception on clock tick if value more than 4 bits") {
            bus->write(16);
            programCounter.jump();

            CHECK_THROWS_WITH(clock.clockTicked(), "ProgramCounter: address out of bounds 16");
        }

        SUBCASE("observer should be notified when jumping") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            programCounter.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).Return();

            bus->write(8);

            programCounter.jump();

            fakeit::VerifyNoOtherInvocations(observerMock); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            fakeit::Verify(Method(observerMock, valueUpdated).Using(6)).Once();
        }

        SUBCASE("reset() should start counting at 0 again") {
            bus->write(8);

            programCounter.jump();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(bus->read(), 8);

            programCounter.reset();

            programCounter.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("observer should be notified when resetting") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            programCounter.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).AlwaysReturn();

            bus->write(8);

            programCounter.jump();
            clock.clockTicked();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(8)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);

            programCounter.reset();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("print() should not fail") {
            programCounter.print();
        }
    }
}
