#include <doctest.h>

#include "core/TimeSource.h"

using namespace Core;

// 1 microsecond = 1000 nanoseconds
double microToNano(double microseconds) {
    return microseconds * 1000.0;
}

TEST_SUITE("TimeSourceTest") {
    TEST_CASE("delta() should return time difference between calls") {
        TimeSource timeSource;

        // Not sure if this is reliable, but works atm
        double startCount = timeSource.delta();
        CHECK(startCount < 1000);

        timeSource.sleep(100);

        // Not easy to verify sleep, but should have slept at least 100 microseconds, and at most 200 hopefully
        double stopCount = timeSource.delta();
        CHECK(stopCount > (startCount + microToNano(100)));
        CHECK(stopCount < (startCount + microToNano(200)));
    }

    TEST_CASE("reset() should reset delta") {
        TimeSource timeSource;

        timeSource.sleep(100);
        timeSource.reset();

        double count = timeSource.delta();
        CHECK(count < 1000);
    }
}
