#ifndef INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTERMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTERMODEL_H

#include <string>

#include "../core/FlagsRegisterObserver.h"

namespace UI {

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
