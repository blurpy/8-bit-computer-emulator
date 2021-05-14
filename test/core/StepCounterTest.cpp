#include <doctest.h>
#include <fakeit.hpp>

#include "core/StepCounter.h"

using namespace Core;

TEST_SUITE("StepCounterTest") {
    TEST_CASE("step counter should work") {
        fakeit::Mock<StepListener> stepListenerMock;
        auto stepListenerMockSharedPtr = std::shared_ptr<StepListener>(&stepListenerMock(), [](...) {});
        StepCounter stepCounter(stepListenerMockSharedPtr);

        fakeit::When(Method(stepListenerMock, stepReady)).AlwaysReturn();

        auto &clock = dynamic_cast<ClockListener&>(stepCounter);

        SUBCASE("inverted clock ticks should notify listener of incrementing steps and wrap at 4") {
            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(2)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(3)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(4)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Twice();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Twice();
        }

        SUBCASE("reset() should start counting steps at 0 again") {
            clock.invertedClockTicked();
            clock.invertedClockTicked();

            fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Once();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Once();

            stepCounter.reset();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Twice();
        }

        SUBCASE("print() should not fail") {
            stepCounter.print();
        }
    }
}
