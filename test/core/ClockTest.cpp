#include <doctest.h>
#include <fakeit.hpp>

#include "core/Clock.h"

using namespace Core;

// 1 millisecond = 1 000 000 nanoseconds
double milliToNano(double milliseconds) {
    return milliseconds * 1000000.0;
}

TEST_SUITE("ClockTest") {
    TEST_CASE("clock should work correctly") {
        fakeit::Mock<TimeSource> timeSourceMock;
        auto timeSourcePtr = std::shared_ptr<TimeSource>(&timeSourceMock(), [](...) {});
        fakeit::Mock<ClockListener> listenerMock;
        auto listenerPtr = std::shared_ptr<ClockListener>(&listenerMock(), [](...) {});

        Clock clock(timeSourcePtr);
        clock.addListener(listenerPtr);

        fakeit::When(Method(timeSourceMock, delta)).AlwaysReturn(milliToNano(100));
        fakeit::When(Method(timeSourceMock, sleep)).AlwaysReturn();
        fakeit::When(Method(timeSourceMock, reset)).AlwaysReturn();
        fakeit::When(Method(listenerMock, clockTicked)).AlwaysReturn();
        fakeit::When(Method(listenerMock, invertedClockTicked)).AlwaysReturn();

        SUBCASE("singleStep() should notify listener of falling and rising edge in the correct time") {
            clock.setFrequency(1);
            clock.singleStep();

            // 1 Hz = 1 clock cycle per second
            // 1 cycle is 2 completed ticks - 1 rising edge (clock tick) and 1 falling edge (inverted clock tick)
            // 1 time delta takes 100 ms (0.1s)
            // To spend 1 second on this single step, we have to ask for delta and sleep 10 times

            // Clock tick must be the first, and then inverted clock tick
            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).Once();
            fakeit::Verify(Method(timeSourceMock, delta)).Exactly(10);
            fakeit::Verify(Method(timeSourceMock, sleep)).Exactly(10);
            fakeit::Verify(Method(timeSourceMock, reset)).Once();
        }
    }
}
