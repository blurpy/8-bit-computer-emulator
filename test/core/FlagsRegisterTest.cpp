#include <doctest.h>
#include <fakeit.hpp>

#include "core/FlagsRegister.h"

using namespace Core;

TEST_SUITE("FlagsRegisterTest") {
    TEST_CASE("flags should be false on new instance") {
        fakeit::Mock<ArithmeticLogicUnit> aluMock;
        auto aluMockSharedPtr = std::shared_ptr<ArithmeticLogicUnit>(&aluMock(), [](...) {});
        FlagsRegister flagsRegister(aluMockSharedPtr);

        CHECK_FALSE(flagsRegister.isCarryFlag());
        CHECK_FALSE(flagsRegister.isZeroFlag());
    }

    TEST_CASE("clockTicked() should not update flags without in()") {
        fakeit::Mock<ArithmeticLogicUnit> aluMock;
        auto aluMockSharedPtr = std::shared_ptr<ArithmeticLogicUnit>(&aluMock(), [](...) {});
        FlagsRegister flagsRegister(aluMockSharedPtr);

        fakeit::When(Method(aluMock, isCarry)).Return(true);
        fakeit::When(Method(aluMock, isZero)).Return(true);

        CHECK_FALSE(flagsRegister.isCarryFlag());
        CHECK_FALSE(flagsRegister.isZeroFlag());

        auto &clock = dynamic_cast<ClockListener&>(flagsRegister);
        clock.clockTicked();

        CHECK_FALSE(flagsRegister.isCarryFlag());
        CHECK_FALSE(flagsRegister.isZeroFlag());
    }

    TEST_CASE("clockTicked() should update flags after in()") {
        fakeit::Mock<ArithmeticLogicUnit> aluMock;
        auto aluMockSharedPtr = std::shared_ptr<ArithmeticLogicUnit>(&aluMock(), [](...) {});
        FlagsRegister flagsRegister(aluMockSharedPtr);

        fakeit::When(Method(aluMock, isCarry)).Return(true);
        fakeit::When(Method(aluMock, isZero)).Return(true);

        flagsRegister.in();

        CHECK_FALSE(flagsRegister.isCarryFlag());
        CHECK_FALSE(flagsRegister.isZeroFlag());

        auto &clock = dynamic_cast<ClockListener&>(flagsRegister);
        clock.clockTicked();

        CHECK(flagsRegister.isCarryFlag());
        CHECK(flagsRegister.isZeroFlag());

        SUBCASE("reset() should set flags to false") {
            flagsRegister.reset();

            CHECK_FALSE(flagsRegister.isCarryFlag());
            CHECK_FALSE(flagsRegister.isZeroFlag());
        }
    }

    TEST_CASE("print() should not fail") {
        fakeit::Mock<ArithmeticLogicUnit> aluMock;
        auto aluMockSharedPtr = std::shared_ptr<ArithmeticLogicUnit>(&aluMock(), [](...) {});
        FlagsRegister flagsRegister(aluMockSharedPtr);

        flagsRegister.print();
    }
}
