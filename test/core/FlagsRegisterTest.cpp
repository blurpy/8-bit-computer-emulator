#include <doctest.h>
#include <fakeit.hpp>

#include "core/FlagsRegister.h"

using namespace Core;

TEST_SUITE("FlagsRegisterTest") {
    TEST_CASE("flags register should work correctly") {
        fakeit::Mock<ArithmeticLogicUnit> aluMock;
        auto aluMockSharedPtr = std::shared_ptr<ArithmeticLogicUnit>(&aluMock(), [](...) {});
        FlagsRegister flagsRegister(aluMockSharedPtr);

        auto &clock = dynamic_cast<ClockListener&>(flagsRegister);

        CHECK_FALSE(flagsRegister.isCarryFlag());
        CHECK_FALSE(flagsRegister.isZeroFlag());

        SUBCASE("clockTicked() should not update flags without in()") {
            fakeit::When(Method(aluMock, isCarry)).Return(true);
            fakeit::When(Method(aluMock, isZero)).Return(true);

            CHECK_FALSE(flagsRegister.isCarryFlag());
            CHECK_FALSE(flagsRegister.isZeroFlag());

            clock.clockTicked();

            CHECK_FALSE(flagsRegister.isCarryFlag());
            CHECK_FALSE(flagsRegister.isZeroFlag());
        }

        SUBCASE("in() should update flags on clock tick") {
            fakeit::When(Method(aluMock, isCarry)).Return(true);
            fakeit::When(Method(aluMock, isZero)).Return(true);

            flagsRegister.in();

            CHECK_FALSE(flagsRegister.isCarryFlag());
            CHECK_FALSE(flagsRegister.isZeroFlag());

            clock.clockTicked();

            CHECK(flagsRegister.isCarryFlag());
            CHECK(flagsRegister.isZeroFlag());

            SUBCASE("reset() should set flags to false") {
                flagsRegister.reset();

                CHECK_FALSE(flagsRegister.isCarryFlag());
                CHECK_FALSE(flagsRegister.isZeroFlag());
            }
        }

        SUBCASE("in() should only update flags on first clock tick") {
            fakeit::When(Method(aluMock, isCarry)).Return(true);
            fakeit::When(Method(aluMock, isZero)).Return(true);

            flagsRegister.in();

            CHECK_FALSE(flagsRegister.isCarryFlag());
            CHECK_FALSE(flagsRegister.isZeroFlag());

            clock.clockTicked();

            CHECK(flagsRegister.isCarryFlag());
            CHECK(flagsRegister.isZeroFlag());

            fakeit::When(Method(aluMock, isCarry)).Return(false);
            fakeit::When(Method(aluMock, isZero)).Return(false);

            clock.clockTicked();

            // Nothing happened
            CHECK(flagsRegister.isCarryFlag());
            CHECK(flagsRegister.isZeroFlag());
        }

        SUBCASE("observer should be notified when reading from the alu") {
            fakeit::Mock<FlagsRegisterObserver> observerMock;
            auto observerPtr = std::shared_ptr<FlagsRegisterObserver>(&observerMock(), [](...) {});
            flagsRegister.setObserver(observerPtr);
            fakeit::When(Method(observerMock, flagsUpdated)).Return();

            fakeit::When(Method(aluMock, isCarry)).Return(true);
            fakeit::When(Method(aluMock, isZero)).Return(false);

            flagsRegister.in();

            fakeit::VerifyNoOtherInvocations(observerMock); // Nothing read from the alu yet

            clock.clockTicked();

            fakeit::Verify(Method(observerMock, flagsUpdated).Using(true, false)).Once();
        }

        SUBCASE("observer should be notified when resetting") {
            fakeit::Mock<FlagsRegisterObserver> observerMock;
            auto observerPtr = std::shared_ptr<FlagsRegisterObserver>(&observerMock(), [](...) {});
            flagsRegister.setObserver(observerPtr);
            fakeit::When(Method(observerMock, flagsUpdated)).AlwaysReturn();

            fakeit::When(Method(aluMock, isCarry)).Return(true);
            fakeit::When(Method(aluMock, isZero)).Return(false);

            flagsRegister.in();
            clock.clockTicked();

            fakeit::Verify(Method(observerMock, flagsUpdated).Using(true, false)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);

            flagsRegister.reset();

            fakeit::Verify(Method(observerMock, flagsUpdated).Using(false, false)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("print() should not fail") {
            flagsRegister.print();
        }
    }
}
