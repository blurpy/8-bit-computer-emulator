#include <doctest.h>

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

    TEST_CASE("reset() should set value to 0") {
        Bus bus = Bus();

        bus.write(5);
        CHECK(bus.read() == 5);

        bus.reset();
        CHECK(bus.read() == 0);
    }

    TEST_CASE("print() should not fail") {
        Bus bus = Bus();

        bus.print();
    }
}
