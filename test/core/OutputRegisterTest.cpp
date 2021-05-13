#include <doctest.h>
#include <fakeit.hpp>

#include "core/OutputRegister.h"

using namespace Core;

TEST_SUITE("OutputRegisterTest") {
    TEST_CASE("output register should work correctly") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();
        OutputRegister outputRegister(bus);

        fakeit::Mock<ValueObserver> observerMock;
        auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
        outputRegister.setObserver(observerPtr);

        auto &clock = dynamic_cast<ClockListener&>(outputRegister);

        CHECK_EQ(bus->read(), 0);
        fakeit::When(Method(observerMock, valueUpdated)).AlwaysReturn();

        SUBCASE("clockTicked() should store value from bus on clock tick") {
            bus->write(8);

            outputRegister.in(); // Prepare to read from bus

            fakeit::VerifyNoOtherInvocations(observerMock); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            fakeit::Verify(Method(observerMock, valueUpdated).Using(6)).Once();
        }

        SUBCASE("clockTicked() should only store on first clock tick") {
            bus->write(15);

            outputRegister.in();
            clock.clockTicked();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(15)).Once();

            bus->write(6);
            clock.clockTicked();

            fakeit::VerifyNoOtherInvocations(observerMock); // No change
        }

        SUBCASE("clockTicked() should handle missing observer") {
            outputRegister.setObserver(nullptr);

            bus->write(150);

            outputRegister.in();
            clock.clockTicked();

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("reset() should reset value to 0") {
            outputRegister.reset();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once();
        }

        SUBCASE("reset() should handle missing observer") {
            outputRegister.setObserver(nullptr);

            outputRegister.reset();

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("print() should not fail") {
            outputRegister.print();
        }
    }
}
