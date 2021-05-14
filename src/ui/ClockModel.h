#ifndef INC_8_BIT_COMPUTER_EMULATOR_CLOCKMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_CLOCKMODEL_H

#include <string>

#include "../core/ClockObserver.h"

namespace UI {

    class ClockModel : public Core::ClockObserver {

    public:
        ClockModel();
        ~ClockModel();

        [[nodiscard]] std::string getRenderText() const;

    private:
        bool on;

        void clockTicked(bool newOn) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_CLOCKMODEL_H
