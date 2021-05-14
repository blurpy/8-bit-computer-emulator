#include <doctest.h>
#include <fakeit.hpp>

#include "core/Utils.h"
#include "core/MemoryAddressRegister.h"

using namespace Core;

TEST_SUITE("MemoryAddressRegisterTest") {
    TEST_CASE("mar should work correctly") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();
        fakeit::Mock<RegisterListener> listenerMock;
        auto listenerPtr = std::shared_ptr<RegisterListener>(&listenerMock(), [](...) {});

        MemoryAddressRegister mar(listenerPtr, bus);

        auto &clock = dynamic_cast<ClockListener&>(mar);

        CHECK_EQ(bus->read(), 0);

        fakeit::When(Method(listenerMock, registerValueChanged)).Return();

        SUBCASE("program should notify listener of programmed value") {
            mar.program(Utils::to4bits(13));

            fakeit::Verify(Method(listenerMock, registerValueChanged).Using(13)).Once();
        }

        SUBCASE("program should notify observer of programmed value") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            mar.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).Return();

            mar.program(Utils::to4bits(14));

            fakeit::Verify(Method(observerMock, valueUpdated).Using(14)).Once();
        }

        SUBCASE("clockTicked() should store value from bus on clock tick") {
            bus->write(8);

            mar.in(); // Prepare to read from bus

            fakeit::VerifyNoOtherInvocations(listenerMock); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            fakeit::Verify(Method(listenerMock, registerValueChanged).Using(6)).Once();
        }

        SUBCASE("clockTicked() should only store on first clock tick") {
            bus->write(15);

            mar.in();
            clock.clockTicked();

            fakeit::Verify(Method(listenerMock, registerValueChanged).Using(15)).Once();

            bus->write(6);
            clock.clockTicked();

            fakeit::VerifyNoOtherInvocations(listenerMock); // No change
        }

        SUBCASE("clockTicked() should throw exception on clock tick if value more than 4 bits") {
            bus->write(16);
            mar.in();

            CHECK_THROWS_WITH(clock.clockTicked(), "MemoryAddressRegister: address out of bounds 16");
        }

        SUBCASE("observer should be notified when reading from the bus") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            mar.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).Return();

            bus->write(8);

            mar.in(); // Prepare to read from bus

            fakeit::VerifyNoOtherInvocations(observerMock); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            fakeit::Verify(Method(observerMock, valueUpdated).Using(6)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("reset() should reset address") {
            mar.reset();

            // Don't currently have a way to verify. Maybe notify listener? Not sure if there is any point.
            fakeit::VerifyNoOtherInvocations(listenerMock);
        }

        SUBCASE("print() should not fail") {
            mar.print();
        }
    }
}
