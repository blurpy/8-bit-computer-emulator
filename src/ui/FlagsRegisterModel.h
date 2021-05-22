#ifndef INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTERMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTERMODEL_H

#include <string>

#include "../core/FlagsRegisterObserver.h"

namespace UI {

    /**
     * Observes the core flags register and prepares the state for presentation in the user interface.
     */
    class FlagsRegisterModel : public Core::FlagsRegisterObserver {

    public:
        FlagsRegisterModel();
        ~FlagsRegisterModel();

        [[nodiscard]] std::string getRenderText() const;

    private:
        bool carryFlag;
        bool zeroFlag;

        void flagsUpdated(bool newCarryFlag, bool newZeroFlag) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTERMODEL_H
