#include <doctest.h>
#include <fakeit.hpp>

#include "core/GenericRegister.h"

using namespace Core;

TEST_SUITE("GenericRegisterTest") {
    TEST_CASE("generic register should work correctly") {
        const std::shared_ptr<Bus> &bus = std::make_shared<Bus>();
        GenericRegister genericRegister("some", bus);
        auto &clock = dynamic_cast<ClockListener &>(genericRegister);

        CHECK_EQ(bus->read(), 0);

        SUBCASE("out() should put 0 on the bus on new instance") {
            genericRegister.out();

            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("clock tick should do nothing by itself") {
            bus->write(8);

            clock.clockTicked();

            genericRegister.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("in() should store value from bus on clock tick") {
            bus->write(8);

            genericRegister.in();

            genericRegister.out();
            CHECK_EQ(bus->read(), 0); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            genericRegister.out();
            CHECK_EQ(bus->read(), 6);
        }

        SUBCASE("in() should only store on first clock tick") {
            bus->write(15);

            genericRegister.in();
            clock.clockTicked();

            genericRegister.out();
            CHECK_EQ(bus->read(), 15);

            bus->write(6);
            clock.clockTicked();

            genericRegister.out();
            CHECK_EQ(bus->read(), 15); // No change
        }

        SUBCASE("listener should be notified when reading from the bus") {
            fakeit::Mock<RegisterListener> listenerMock;
            auto listenerPtr = std::shared_ptr<RegisterListener>(&listenerMock(), [](...) {});
            genericRegister.setRegisterListener(listenerPtr);
            fakeit::When(Method(listenerMock, registerValueChanged)).Return();

            bus->write(8);

            genericRegister.in(); // Prepare to read from bus

            fakeit::VerifyNoOtherInvocations(listenerMock); // Nothing read from the bus yet

            bus->write(6); // This should be stored
            clock.clockTicked();

            bus->write(4); // Change the bus after storing the last value, otherwise it's difficult to know

            fakeit::Verify(Method(listenerMock, registerValueChanged).Using(6)).Once();
        }

        SUBCASE("reset() should set value to 0 again") {
            bus->write(8);

            genericRegister.in();
            clock.clockTicked();

            genericRegister.out();
            CHECK_EQ(bus->read(), 8);

            genericRegister.reset();

            genericRegister.out();
            CHECK_EQ(bus->read(), 0);
        }

        SUBCASE("print() should not fail") {
            genericRegister.print();
        }

        SUBCASE("readValue() should return after in() and clock tick") {
            bus->write(230);

            genericRegister.in();

            CHECK_EQ(genericRegister.readValue(), 0); // Nothing yet

            clock.clockTicked();

            CHECK_EQ(genericRegister.readValue(), 230);
        }
    }
}
