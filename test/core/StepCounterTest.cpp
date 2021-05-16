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
            // First step is from 0 -> 1
            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(2)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(3)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(4)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Twice();

            fakeit::VerifyNoOtherInvocations(stepListenerMock);
        }

        SUBCASE("inverted clock ticks should notify observer of incrementing steps") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            stepCounter.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).AlwaysReturn();

            clock.invertedClockTicked();
            fakeit::Verify(Method(observerMock, valueUpdated).Using(1)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(observerMock, valueUpdated).Using(2)).Once();

            clock.invertedClockTicked();
            fakeit::Verify(Method(observerMock, valueUpdated).Using(3)).Once();

            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("reset() should start at 0 and notify observer") {
            fakeit::Mock<ValueObserver> observerMock;
            auto observerPtr = std::shared_ptr<ValueObserver>(&observerMock(), [](...) {});
            stepCounter.setObserver(observerPtr);
            fakeit::When(Method(observerMock, valueUpdated)).AlwaysReturn();

            clock.invertedClockTicked();
            fakeit::Verify(Method(observerMock, valueUpdated).Using(1)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);

            stepCounter.reset();

            fakeit::Verify(Method(observerMock, valueUpdated).Using(0)).Once();
            fakeit::VerifyNoOtherInvocations(observerMock);
        }

        SUBCASE("reset() should start at 0 and notify listener") {
            clock.invertedClockTicked();
            fakeit::Verify(Method(stepListenerMock, stepReady).Using(1)).Once();
            fakeit::VerifyNoOtherInvocations(stepListenerMock);

            stepCounter.reset();

            fakeit::Verify(Method(stepListenerMock, stepReady).Using(0)).Once();
            fakeit::VerifyNoOtherInvocations(stepListenerMock);
        }

        SUBCASE("print() should not fail") {
            stepCounter.print();
        }
    }
}
