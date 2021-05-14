#include <doctest.h>

#include "core/ProgramCounter.h"

using namespace Core;

TEST_SUITE("ProgramCounterTest") {
    TEST_CASE("program counter should work") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);

        auto &clock = dynamic_cast<ClockListener&>(programCounter);

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 0);

        SUBCASE("clock tick should do nothing by itself") {
            busSharedPtr->write(8);

            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 0); // No jump, and no increment
        }

        SUBCASE("enable() should increment on clock tick") {
            programCounter.enable();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 0); // Nothing happened yet

            clock.clockTicked();

            CHECK_EQ(busSharedPtr->read(), 0);
            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 1);
        }

        SUBCASE("enable() should only increment on first clock tick") {
            programCounter.enable();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 1);

            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 1); // No change
        }

        SUBCASE("enable() should wrap around") {
            for (int i = 0; i < 15; i++) {
                programCounter.enable();
                clock.clockTicked();
            }

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 15);

            programCounter.enable();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 0);
        }

        SUBCASE("jump() should change counter to value from bus on clock tick") {
            busSharedPtr->write(8);

            programCounter.jump();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 0); // Nothing read from the bus yet

            busSharedPtr->write(6); // This should be stored
            clock.clockTicked();

            busSharedPtr->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 6);
        }

        SUBCASE("jump() should only change on first clock tick") {
            busSharedPtr->write(15);

            programCounter.jump();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 15);

            busSharedPtr->write(6);
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 15); // No change
        }

        SUBCASE("jump() should throw exception on clock tick if value more than 4 bits") {
            busSharedPtr->write(16);
            programCounter.jump();

            CHECK_THROWS_WITH(clock.clockTicked(), "ProgramCounter: address out of bounds 16");
        }

        SUBCASE("reset() should start counting at 0 again") {
            busSharedPtr->write(8);

            programCounter.jump();
            clock.clockTicked();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 8);

            programCounter.reset();

            programCounter.out();
            CHECK_EQ(busSharedPtr->read(), 0);
        }

        SUBCASE("print() should not fail") {
            programCounter.print();
        }
    }
}
