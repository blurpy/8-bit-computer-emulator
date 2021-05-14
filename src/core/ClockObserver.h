#ifndef INC_8_BIT_COMPUTER_EMULATOR_CLOCKOBSERVER_H
#define INC_8_BIT_COMPUTER_EMULATOR_CLOCKOBSERVER_H

namespace Core {

    /**
     * Interface for external observation of the clock of the computer.
     */
    class ClockObserver {

    public:
        /** The clock has ticked either on or off. */
        virtual void clockTicked(bool newOn) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_CLOCKOBSERVER_H
