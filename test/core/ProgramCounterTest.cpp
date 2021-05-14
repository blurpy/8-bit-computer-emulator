#include <doctest.h>

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

        SUBCASE("print() should not fail") {
            programCounter.print();
        }
    }
}
