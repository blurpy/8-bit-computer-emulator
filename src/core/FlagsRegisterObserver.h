#ifndef INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTEROBSERVER_H
#define INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTEROBSERVER_H

namespace Core {

    /**
     * Interface for external observation of the flags of the computer.
     */
    class FlagsRegisterObserver {

    public:
        /** The flags have been updated. */
        virtual void flagsUpdated(bool newCarryFlag, bool newZeroFlag) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTEROBSERVER_H
