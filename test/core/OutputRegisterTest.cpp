#include <doctest.h>

#include "core/OutputRegister.h"

using namespace Core;

TEST_SUITE("OutputRegisterTest") {
    TEST_CASE("clockTicked() should store value from bus on clock tick") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();
        OutputRegister outputRegister(bus);
        auto &clock = dynamic_cast<ClockListener &>(outputRegister);

        bus->write(130);

        outputRegister.in();
        clock.clockTicked();

        // There is not api for getting the value out of the register, so this test must be inspected visually
        outputRegister.print();

        SUBCASE("reset() should reset value to 0") {
            outputRegister.reset();

            outputRegister.print();
        }
    }
}
