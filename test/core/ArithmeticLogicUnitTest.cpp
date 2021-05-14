#include <doctest.h>
#include <fakeit.hpp>

#include "core/ArithmeticLogicUnit.h"

using namespace Core;

TEST_SUITE("ArithmeticLogicUnitTest") {
    TEST_CASE("only zero bit should be true on new instance") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();
        fakeit::Mock<GenericRegister> aRegisterMock;
        auto aRegisterMockPtr = std::shared_ptr<GenericRegister>(&aRegisterMock(), [](...) {});
        fakeit::Mock<GenericRegister> bRegisterMock;
        auto bRegisterMockPtr = std::shared_ptr<GenericRegister>(&bRegisterMock(), [](...) {});

        fakeit::When(Method(aRegisterMock, readValue)).Return(0);
        fakeit::When(Method(bRegisterMock, readValue)).Return(0);

        ArithmeticLogicUnit alu(aRegisterMockPtr, bRegisterMockPtr, bus);
        auto &theRegister = dynamic_cast<RegisterListener&>(alu);

        CHECK_FALSE(alu.isCarry());
        CHECK(alu.isZero());
        CHECK_EQ(bus->read(), 0);

        SUBCASE("registerValueChanged() should trigger addition from a and b registers") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(8);
            fakeit::When(Method(bRegisterMock, readValue)).Return(6);

            theRegister.registerValueChanged(0);

            alu.out();
            CHECK_EQ(bus->read(), 14);
        }

        SUBCASE("addition should set only zero true if result is 0") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(0);
            fakeit::When(Method(bRegisterMock, readValue)).Return(0);

            theRegister.registerValueChanged(0);

            CHECK_FALSE(alu.isCarry());
            CHECK(alu.isZero());

            alu.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("addition should set both carry and zero false when positive and not wrapped result") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(50);
            fakeit::When(Method(bRegisterMock, readValue)).Return(13);

            theRegister.registerValueChanged(0);

            CHECK_FALSE(alu.isCarry());
            CHECK_FALSE(alu.isZero());

            alu.out();
            CHECK_EQ(bus->read(), 63);
        }

        SUBCASE("addition should set both carry and zero true when wrap around to exactly 0") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(255);
            fakeit::When(Method(bRegisterMock, readValue)).Return(1);

            theRegister.registerValueChanged(0);

            CHECK(alu.isCarry());
            CHECK(alu.isZero());

            alu.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("addition should set only carry true when wrap around is more than 0") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(250);
            fakeit::When(Method(bRegisterMock, readValue)).Return(10);

            theRegister.registerValueChanged(0);

            CHECK(alu.isCarry());
            CHECK_FALSE(alu.isZero());

            alu.out();
            CHECK_EQ(bus->read(), 4);

            SUBCASE("reset() should set value to 0 and only zero bit to true") {
                alu.reset();

                CHECK_FALSE(alu.isCarry());
                CHECK(alu.isZero());

                alu.out();
                CHECK_EQ(bus->read(), 0);
            }
        }

        SUBCASE("addition should notify observer") {
            fakeit::Mock<ArithmeticLogicUnitObserver> observerMock;
            auto observerPtr = std::shared_ptr<ArithmeticLogicUnitObserver>(&observerMock(), [](...) {});
            alu.setObserver(observerPtr);
            fakeit::When(Method(observerMock, resultUpdated)).Return();

            fakeit::When(Method(aRegisterMock, readValue)).Return(250);
            fakeit::When(Method(bRegisterMock, readValue)).Return(10);

            theRegister.registerValueChanged(0);

            CHECK(alu.isCarry());
            CHECK_FALSE(alu.isZero());

            fakeit::Verify(Method(observerMock, resultUpdated).Using(4, true, false)).Once();
        }

        SUBCASE("subtract() should trigger subtraction from a and b registers") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(8);
            fakeit::When(Method(bRegisterMock, readValue)).Return(6);

            alu.subtract();

            alu.out();
            CHECK_EQ(bus->read(), 2);
        }

        SUBCASE("subtract() should set carry for positive result") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(30);
            fakeit::When(Method(bRegisterMock, readValue)).Return(12);

            alu.subtract();

            CHECK(alu.isCarry());
            CHECK_FALSE(alu.isZero());

            alu.out();
            CHECK_EQ(bus->read(), 18);
        }

        SUBCASE("subtract() should not set carry when wrapping around") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(0);
            fakeit::When(Method(bRegisterMock, readValue)).Return(1);

            alu.subtract();

            CHECK_FALSE(alu.isCarry());
            CHECK_FALSE(alu.isZero());

            alu.out();
            CHECK_EQ(bus->read(), 255);
        }

        SUBCASE("subtract() should set zero when result is 0") {
            fakeit::When(Method(aRegisterMock, readValue)).Return(1);
            fakeit::When(Method(bRegisterMock, readValue)).Return(1);

            alu.subtract();

            CHECK(alu.isCarry());
            CHECK(alu.isZero());

            alu.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("subtract() should notify observer") {
            fakeit::Mock<ArithmeticLogicUnitObserver> observerMock;
            auto observerPtr = std::shared_ptr<ArithmeticLogicUnitObserver>(&observerMock(), [](...) {});
            alu.setObserver(observerPtr);
            fakeit::When(Method(observerMock, resultUpdated)).Return();

            fakeit::When(Method(aRegisterMock, readValue)).Return(1);
            fakeit::When(Method(bRegisterMock, readValue)).Return(1);

            alu.subtract();

            CHECK(alu.isCarry());
            CHECK(alu.isZero());

            fakeit::Verify(Method(observerMock, resultUpdated).Using(0, true, true)).Once();
        }

        SUBCASE("print() should not fail") {
            alu.print();
        }
    }
}
