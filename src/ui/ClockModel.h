#ifndef INC_8_BIT_COMPUTER_EMULATOR_CLOCKMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_CLOCKMODEL_H

#include <string>

#include "../core/ClockObserver.h"

namespace UI {

    /**
     * Observes the core clock and prepares the state for presentation in the user interface.
     */
    class ClockModel : public Core::ClockObserver {

    public:
        ClockModel();
        ~ClockModel();

        [[nodiscard]] std::string getRenderText() const;

    private:
        bool on;
        double frequency;

        void clockTicked(bool newOn) override;
        void frequencyChanged(double newHz) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_CLOCKMODEL_H
