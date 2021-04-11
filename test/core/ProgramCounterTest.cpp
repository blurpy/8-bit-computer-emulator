#include <doctest.h>

#include "core/ProgramCounter.h"

TEST_SUITE("ProgramCounterTest") {
    TEST_CASE("out() should put 0 on the bus on new instance") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);

        programCounter.out();

        CHECK_EQ(busSharedPtr->read(), 0);
    }

    TEST_CASE("clock tick should do nothing by itself") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);
        auto &clock = dynamic_cast<ClockListener &>(programCounter);

        busSharedPtr->write(8);

        clock.clockTicked();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 0); // No jump, and no increment
    }

    TEST_CASE("enable() should increment on clock tick") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);
        auto &clock = dynamic_cast<ClockListener &>(programCounter);

        programCounter.enable();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 0); // Nothing happened yet

        clock.clockTicked();

        CHECK_EQ(busSharedPtr->read(), 0);
        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 1);
    }

    TEST_CASE("enable() should only increment on first clock tick") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);
        auto &clock = dynamic_cast<ClockListener &>(programCounter);

        programCounter.enable();
        clock.clockTicked();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 1);

        clock.clockTicked();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 1); // No change
    }

    TEST_CASE("enable() should wrap around") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);
        auto &clock = dynamic_cast<ClockListener &>(programCounter);

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

    TEST_CASE("jump() should change counter to value from bus on clock tick") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);
        auto &clock = dynamic_cast<ClockListener &>(programCounter);

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

    TEST_CASE("jump() should only change on first clock tick") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);
        auto &clock = dynamic_cast<ClockListener &>(programCounter);

        busSharedPtr->write(8);

        programCounter.jump();
        clock.clockTicked();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 8);

        busSharedPtr->write(6);
        clock.clockTicked();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 8); // No change
    }

    TEST_CASE("reset() should start counting at 0 again") {
        const std::shared_ptr<Bus> &busSharedPtr = std::make_shared<Bus>();
        ProgramCounter programCounter(busSharedPtr);
        auto &clock = dynamic_cast<ClockListener &>(programCounter);

        busSharedPtr->write(8);

        programCounter.jump();
        clock.clockTicked();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 8);

        programCounter.reset();

        programCounter.out();
        CHECK_EQ(busSharedPtr->read(), 0);
    }
}
