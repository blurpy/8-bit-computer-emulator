#include <doctest.h>
#include <fakeit.hpp>

#include <bitset>

#include "core/InstructionRegister.h"

using namespace Core;

TEST_SUITE("InstructionRegisterTest") {
    TEST_CASE("instruction register should work correctly") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();
        InstructionRegister instructionRegister(bus);
        auto &clock = dynamic_cast<ClockListener &>(instructionRegister);

        CHECK_EQ(bus->read(), 0);

        SUBCASE("out() should put 0 on the bus on new instance") {
            instructionRegister.out();

            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("clock tick should do nothing by itself") {
            bus->write(8);

            clock.clockTicked();

            instructionRegister.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("in() should store value from bus on clock tick") {
            bus->write(8);

            instructionRegister.in();

            instructionRegister.out();
            CHECK_EQ(bus->read(), 0); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            instructionRegister.out();
            CHECK_EQ(bus->read(), 6);
        }

        SUBCASE("in() should only store on first clock tick") {
            bus->write(15);

            instructionRegister.in();
            clock.clockTicked();

            instructionRegister.out();
            CHECK_EQ(bus->read(), 15);

            bus->write(6);
            clock.clockTicked();

            instructionRegister.out();
            CHECK_EQ(bus->read(), 15); // No change
        }

        SUBCASE("observer should be notified when reading from the bus") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            instructionRegister.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).Return();

            bus->write(8);

            instructionRegister.in(); // Prepare to read from bus

            fakeit::VerifyNoOtherInvocations(observerMock); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            fakeit::Verify(Method(observerMock, valueUpdated).Using(6)).Once();
        }

        SUBCASE("reset() should set value to 0 again") {
            bus->write(8);

            instructionRegister.in();
            clock.clockTicked();

            instructionRegister.out();
            CHECK_EQ(bus->read(), 8);

            instructionRegister.reset();

            instructionRegister.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("reset() should notify observer") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            instructionRegister.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).AlwaysReturn();

            bus->write(83);

            instructionRegister.in();
            clock.clockTicked();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(83)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);

            instructionRegister.reset();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("print() should not fail") {
            instructionRegister.print();
        }

        SUBCASE("getOpcode() should return 4 first bits after in() and clock tick") {
            bus->write(std::bitset<8>("11001010").to_ulong()); // 202

            instructionRegister.in();

            CHECK_EQ(instructionRegister.getOpcode(), 0); // Nothing yet

            clock.clockTicked();

            CHECK_EQ(instructionRegister.getOpcode(), std::bitset<4>("1100").to_ulong()); // 12
        }

        SUBCASE("out() should put 4 last bits on the bus") {
            bus->write(std::bitset<8>("11001010").to_ulong()); // 202

            instructionRegister.in();
            clock.clockTicked();

            instructionRegister.out();
            CHECK_EQ(bus->read(), std::bitset<4>("1010").to_ulong()); // 10
        }
    }
}
