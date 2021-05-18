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
        WARN(startCount < 1000);

        timeSource.sleep(microToNano(100));

        // Not easy to verify sleep, but should have slept at least 100 microseconds, and at most 200 hopefully
        double stopCount = timeSource.delta();
        WARN(stopCount > (startCount + microToNano(100)));
        WARN(stopCount < (startCount + microToNano(200)));
    }

    TEST_CASE("delta() should not increase but return difference since last time") {
        TimeSource timeSource;

        // Not sure if this is reliable, but works atm
        for (int i = 0; i < 10; i++) {
            WARN(timeSource.delta() < 1000);
        }
    }

    TEST_CASE("reset() should reset delta") {
        TimeSource timeSource;

        timeSource.sleep(microToNano(100));
        timeSource.reset();

        double count = timeSource.delta();
        WARN(count < 1000);
    }
}
