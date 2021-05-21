#include <doctest.h>
#include <fakeit.hpp>

#include <core/Utils.h>

#include "core/Clock.h"

using namespace Core;

// 1 millisecond = 1 000 000 nanoseconds
double milliToNano(double milliseconds) {
    return milliseconds * 1000000.0;
}

// Workaround for issue with comparison of floating point numbers: https://github.com/eranpeer/FakeIt/issues/218
std::function<bool (double)> equals(const double &expected) {
    return [&](double input) -> bool {
        return Utils::equals(expected, input);
    };
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

            // 1 Hz = 1 clock cycle per second (1 000 000 000 nanoseconds)
            // 1 cycle is 2 completed ticks - 1 rising edge (clock tick) and 1 falling edge (inverted clock tick)
            // 1 time delta takes 100 ms (0.1s / 100 000 000 ns)

            // Clock tick must be the first, and then inverted clock tick
            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).Once();

            // To spend 1 second on this single step, we have to ask for delta 10 times since sleep does nothing
            fakeit::Verify(Method(timeSourceMock, delta)).Exactly(10);

            // It tries to calculate how long to sleep to avoid unnecessary work. In the real world
            // it would sleep 400ms if the delta was 100ms, to spend a total of 500ms on a tick.
            // In this test it counts down by the delta, since the delta is fixed instead of based on time passed.
            fakeit::Verify(Method(timeSourceMock, sleep).Using(400000000),
                           Method(timeSourceMock, sleep).Using(300000000),
                           Method(timeSourceMock, sleep).Using(200000000),
                           Method(timeSourceMock, sleep).Using(100000000)).Twice();
            fakeit::Verify(Method(timeSourceMock, reset)).Once();
        }

        SUBCASE("singleStep() should calculate sleep correctly at different speeds ") {
            fakeit::When(Method(timeSourceMock, delta)).AlwaysReturn(milliToNano(10));
            clock.setFrequency(2);
            clock.singleStep();

            // 2 Hz = 2 clock cycles per second
            // 1 time delta takes 10 ms (0.01s)
            // To spend 500 ms on this single step, we have to ask for delta 50 times

            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).Once();
            fakeit::Verify(Method(timeSourceMock, delta)).Exactly(50);
            fakeit::Verify(Method(timeSourceMock, sleep)).Exactly(48); // Doesn't sleep when it ticks
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
            fakeit::Verify(Method(timeSourceMock, sleep)).Never(); // No need to sleep
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
            fakeit::Verify(Method(timeSourceMock, reset)).Exactly(3);
        }

        SUBCASE("singleStep() should notify observer") {
            fakeit::Mock<ClockObserver> observerMock;
            auto observerPtr = std::shared_ptr<ClockObserver>(&observerMock(), [](...) {});
            clock.setObserver(observerPtr);
            fakeit::When(Method(observerMock, clockTicked)).AlwaysReturn();
            fakeit::When(Method(observerMock, frequencyChanged)).Return();

            clock.setFrequency(5000);

            clock.singleStep();
            clock.singleStep();

            // First on, then off - two times
            fakeit::Verify(Method(observerMock, clockTicked).Using(true),
                           Method(observerMock, clockTicked).Using(false)).Twice();
        }

        SUBCASE("singleStep() should reset running flag") {
            CHECK_FALSE(clock.isRunning());

            clock.setFrequency(5000);
            clock.singleStep(); // Should be true right now, but can't verify

            CHECK_FALSE(clock.isRunning());
        }

        SUBCASE("singleStep() should do nothing when already running") {
            clock.setFrequency(5);
            clock.start();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            clock.singleStep(); // Would crash unless handled

            clock.stop();
            clock.join();
        }

        SUBCASE("clearListeners() should remove all listeners") {
            clock.setFrequency(5000);
            clock.clearListeners();

            clock.singleStep();

            fakeit::VerifyNoOtherInvocations(listenerMock);
            fakeit::Verify(Method(timeSourceMock, delta)).Twice();
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
            fakeit::Verify(Method(timeSourceMock, reset)).Exactly(1);
        }

        SUBCASE("clock should be running between start and stop") {
            CHECK_FALSE(clock.isRunning());

            clock.setFrequency(5);
            clock.start();

            CHECK(clock.isRunning());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            CHECK(clock.isRunning());

            clock.stop();
            clock.join();

            CHECK_FALSE(clock.isRunning());
        }

        SUBCASE("halt() should prevent restarting clock") {
            CHECK_FALSE(clock.isRunning());

            clock.setFrequency(5);
            clock.start();

            CHECK(clock.isRunning());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            clock.halt();
            clock.join();

            CHECK_FALSE(clock.isRunning());

            clock.start();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            CHECK_FALSE(clock.isRunning());
        }

        SUBCASE("halt() should prevent single stepping clock") {
            CHECK_FALSE(clock.isRunning());

            clock.setFrequency(5);
            clock.start();

            CHECK(clock.isRunning());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            clock.halt();
            clock.join();

            CHECK_FALSE(clock.isRunning());
            listenerMock.ClearInvocationHistory();

            clock.singleStep();
            fakeit::VerifyNoOtherInvocations(listenerMock);
        }

        SUBCASE("reset() should allow starting the clock again after halt()") {
            clock.setFrequency(5);
            clock.start();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            clock.halt();
            clock.join();

            CHECK_FALSE(clock.isRunning());
            listenerMock.ClearInvocationHistory();

            clock.reset();

            clock.singleStep();
            fakeit::Verify(Method(listenerMock, clockTicked), Method(listenerMock, invertedClockTicked)).Once();
            fakeit::VerifyNoOtherInvocations(listenerMock);
        }

        SUBCASE("detach() should allow restarting the clock without join()") {
            clock.setFrequency(5);
            clock.start();

            clock.detach();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            clock.stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            CHECK_FALSE(clock.isRunning());

            clock.singleStep();
        }

        SUBCASE("setFrequency() should notify observer") {
            fakeit::Mock<ClockObserver> observerMock;
            auto observerPtr = std::shared_ptr<ClockObserver>(&observerMock(), [](...) {});
            clock.setObserver(observerPtr);
            fakeit::When(Method(observerMock, frequencyChanged)).Return();

            clock.setFrequency(5000);

            fakeit::Verify(Method(observerMock, frequencyChanged).Using(5000));
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

        SUBCASE("increaseFrequency() should increase in different step sizes") {
            fakeit::Mock<ClockObserver> observerMock;
            auto observerPtr = std::shared_ptr<ClockObserver>(&observerMock(), [](...) {});
            clock.setObserver(observerPtr);
            fakeit::When(Method(observerMock, frequencyChanged)).AlwaysReturn();

            clock.setFrequency(0.1); // Start at lowest value

            fakeit::Verify(Method(observerMock, frequencyChanged).Using(0.1));
            fakeit::VerifyNoOtherInvocations(observerMock);

            for (int i = 0; i < 68; i++) {
                clock.increaseFrequency();
            }

            // Increase by 0.1
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.2))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.3))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.4))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.5))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.6))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.7))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.8))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.9))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1))).Once();

            // Increase by 1
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(2))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(3))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(4))).Once();

            for (int i = 5; i <= 17; i++) {
                fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(i))).Once();
            }

            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(18))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(19))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(20))).Once();

            // Increase by 10
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(30))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(40))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(50))).Once();

            for (int i = 60; i <= 170; i += 10) {
                fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(i))).Once();
            }

            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(180))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(190))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(200))).Once();

            // Increase by 100
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(300))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(400))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(500))).Once();

            for (int i = 600; i <= 1700; i += 100) {
                fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(i))).Once();
            }

            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1800))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1900))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(2000))).Once();

            // Increase by 1000
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(3000))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(4000))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(5000))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(6000))).Once();

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("decreaseFrequency() should decrease in different step sizes") {
            fakeit::Mock<ClockObserver> observerMock;
            auto observerPtr = std::shared_ptr<ClockObserver>(&observerMock(), [](...) {});
            clock.setObserver(observerPtr);
            fakeit::When(Method(observerMock, frequencyChanged)).AlwaysReturn();

            clock.setFrequency(6000); // Start at a high value

            fakeit::Verify(Method(observerMock, frequencyChanged).Using(6000));
            fakeit::VerifyNoOtherInvocations(observerMock);

            for (int i = 0; i < 68; i++) {
                clock.decreaseFrequency();
            }

            // Decrease by 1000
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(5000))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(4000))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(3000))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(2000))).Once();

            // Decrease by 100
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1900))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1800))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1700))).Once();

            for (int i = 1600; i >= 500; i -= 100) {
                fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(i))).Once();
            }

            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(400))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(300))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(200))).Once();

            // Decrease by 10
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(190))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(180))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(170))).Once();

            for (int i = 160; i >= 50; i -= 10) {
                fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(i))).Once();
            }

            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(40))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(30))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(20))).Once();

            // Decrease by 1
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(19))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(18))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(17))).Once();

            for (int i = 16; i >= 4; i--) {
                fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(i))).Once();
            }

            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(3))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(2))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1))).Once();

            // Decrease by 0.1
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.9))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.8))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.7))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.6))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.5))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.4))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.3))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.2))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.1))).Once();

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("decreaseFrequency() should not decrease below 0.1") {
            // Try to avoid this:
            // Clock: changing frequency to 0.3
            // Clock: changing frequency to 0.2
            // Clock: changing frequency to 0.1
            // Clock: frequency too low 0.100000

            fakeit::Mock<ClockObserver> observerMock;
            auto observerPtr = std::shared_ptr<ClockObserver>(&observerMock(), [](...) {});
            clock.setObserver(observerPtr);
            fakeit::When(Method(observerMock, frequencyChanged)).AlwaysReturn();

            clock.setFrequency(0.3);
            fakeit::Verify(Method(observerMock, frequencyChanged).Using(0.3));

            clock.decreaseFrequency();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.2))).Once();

            clock.decreaseFrequency();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.1))).Once();

            // Decrease has no effect anymore
            clock.decreaseFrequency();
            clock.decreaseFrequency();
            fakeit::VerifyNoOtherInvocations(observerMock);

            observerMock.ClearInvocationHistory(); // Start fresh

            // But should be able to increase again
            clock.increaseFrequency();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.2))).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("decreaseFrequency() should handle starting from 2Hz") {
            // Confusing, but decreasing from 2Hz in the UI can result in this:
            // Clock: changing frequency to 0.1
            // Clock: changing frequency to 1.38778e-16
            // Clock: frequency too low 0.000000

            fakeit::Mock<ClockObserver> observerMock;
            auto observerPtr = std::shared_ptr<ClockObserver>(&observerMock(), [](...) {});
            clock.setObserver(observerPtr);
            fakeit::When(Method(observerMock, frequencyChanged)).AlwaysReturn();

            clock.setFrequency(2);
            fakeit::Verify(Method(observerMock, frequencyChanged).Using(2));

            for (int i = 0; i < 10; i++) {
                clock.decreaseFrequency();
            }

            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(1))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.9))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.8))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.7))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.6))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.5))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.4))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.3))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.2))).Once();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.1))).Once();

            // Decrease has no effect anymore
            clock.decreaseFrequency();
            clock.decreaseFrequency();
            fakeit::VerifyNoOtherInvocations(observerMock);

            observerMock.ClearInvocationHistory(); // Start fresh

            // But should be able to increase again
            clock.increaseFrequency();
            fakeit::Verify(Method(observerMock, frequencyChanged).Matching(equals(0.2))).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }
    }
}
