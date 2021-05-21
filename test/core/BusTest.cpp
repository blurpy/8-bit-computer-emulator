#include <doctest.h>
#include <fakeit.hpp>

#include "core/Bus.h"

using namespace Core;

TEST_SUITE("BusTest") {
    TEST_CASE("read() should return 0 on new instance") {
        Bus bus = Bus();

        CHECK(bus.read() == 0);
    }

    TEST_CASE("read() should return value set with write()") {
        Bus bus = Bus();
        bus.write(10);

        CHECK(bus.read() == 10);
    }

    TEST_CASE("observer should be notified when writing to the bus") {
        Bus bus = Bus();

        fakeit::Mock<ValueObserver> observerMock;
        auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
        bus.setObserver(observerPtr);
        fakeit::When(Method(observerMock, valueUpdated)).Return();

        bus.write(8);

        fakeit::Verify(Method(observerMock, valueUpdated).Using(8)).Once();
        fakeit::VerifyNoOtherInvocations(observerMock);
    }

    TEST_CASE("reset() should set value to 0") {
        Bus bus = Bus();

        bus.write(5);
        CHECK(bus.read() == 5);

        bus.reset();
        CHECK(bus.read() == 0);
    }

    TEST_CASE("reset() should notify observer") {
        Bus bus = Bus();

        fakeit::Mock<ValueObserver> observerMock;
        auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
        bus.setObserver(observerPtr);
        fakeit::When(Method(observerMock, valueUpdated)).AlwaysReturn();

        bus.write(250);
        fakeit::Verify(Method(observerMock, valueUpdated).Using(250)).Once();
        fakeit::VerifyNoOtherInvocations(observerMock);

        bus.reset();
        fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once();
        fakeit::VerifyNoOtherInvocations(observerMock);
    }

    TEST_CASE("print() should not fail") {
        Bus bus = Bus();

        bus.print();
    }
}
