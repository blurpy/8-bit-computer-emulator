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

        SUBCASE("singleStep() should throw exception if frequency is not set") {
            CHECK_THROWS_WITH(clock.singleStep(), "Clock: frequency must be set before start");
        }

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
            fakeit::Verify(Method(timeSourceMock, sleep).Using(100)).Exactly(10);
            fakeit::Verify(Method(timeSourceMock, reset)).Once();
        }

        SUBCASE("singleStep() should calculate sleep correctly at different speeds ") {
            fakeit::When(Method(timeSourceMock, delta)).AlwaysReturn(milliToNano(10));
            clock.setFrequency(2);
            clock.singleStep();

            // 2 Hz = 2 clock cycles per second
            // 1 time delta takes 10 ms (0.01s)
            // To spend 500 ms on this single step, we have to ask for delta and sleep 50 times

            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).Once();
            fakeit::Verify(Method(timeSourceMock, delta)).Exactly(50);
            fakeit::Verify(Method(timeSourceMock, sleep)).Exactly(50);
            fakeit::Verify(Method(timeSourceMock, reset)).Once();
        }

        SUBCASE("singleStep() should always complete both ticks even at high speeds") {
            clock.setFrequency(5000);
            clock.singleStep();

            // 5000 Hz = 5000 clock cycles per second
            // 1 time delta takes 100 ms (0.1s)
            // At this speed there is no reason to run the wait loop more than once of each tick

            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).Once();
            fakeit::Verify(Method(timeSourceMock, delta)).Twice();
            fakeit::Verify(Method(timeSourceMock, sleep)).Twice();
            fakeit::Verify(Method(timeSourceMock, reset)).Once();
        }

        SUBCASE("singleStep() should be repeatable") {
            clock.setFrequency(5000);

            clock.singleStep();
            clock.singleStep();
            clock.singleStep();

            // Should be 3 times of everything from the previous test
            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).Exactly(3);
            fakeit::Verify(Method(timeSourceMock, delta)).Exactly(6);
            fakeit::Verify(Method(timeSourceMock, sleep)).Exactly(6);
            fakeit::Verify(Method(timeSourceMock, reset)).Exactly(3);
        }

        SUBCASE("clearListeners() should remove all listeners") {
            clock.setFrequency(5000);
            clock.clearListeners();

            clock.singleStep();

            fakeit::VerifyNoOtherInvocations(listenerMock);
            fakeit::Verify(Method(timeSourceMock, delta)).Twice();
            fakeit::Verify(Method(timeSourceMock, sleep)).Twice();
            fakeit::Verify(Method(timeSourceMock, reset)).Once();
        }

        SUBCASE("start() should throw exception if frequency is not set") {
            CHECK_THROWS_WITH(clock.start(), "Clock: frequency must be set before start");
        }

        SUBCASE("start() should run the same as single step but until stop()") {
            clock.setFrequency(5);
            clock.start();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            clock.stop();
            clock.join();

            // This will be many many times since sleep is mocked
            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).AtLeast(100);
            fakeit::Verify(Method(timeSourceMock, delta)).AtLeast(100);
            fakeit::Verify(Method(timeSourceMock, sleep)).AtLeast(100);
            fakeit::Verify(Method(timeSourceMock, reset)).Exactly(1);
        }

        SUBCASE("setFrequency() should throw exception if frequency is set to 0") {
            CHECK_THROWS_WITH(clock.setFrequency(0), "Clock: frequency too low 0.000000");
        }

        SUBCASE("setFrequency() should throw exception if frequency is set negative") {
            CHECK_THROWS_WITH(clock.setFrequency(-1), "Clock: frequency too low -1.000000");
        }

        SUBCASE("setFrequency() should accept frequency of 0.1") {
            clock.setFrequency(0.1);
        }
    }
}
