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

        SUBCASE("clockTicked() should update flags after in()") {
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

        SUBCASE("print() should not fail") {
            flagsRegister.print();
        }
    }
}
