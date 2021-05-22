#ifndef INC_8_BIT_COMPUTER_EMULATOR_ARITHMETICLOGICUNITMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_ARITHMETICLOGICUNITMODEL_H

#include <string>

#include "../core/ArithmeticLogicUnitObserver.h"

namespace UI {

    /**
     * Observes the core arithmetic logic unit and prepares the state for presentation in the user interface.
     */
    class ArithmeticLogicUnitModel : public Core::ArithmeticLogicUnitObserver {

    public:
        ArithmeticLogicUnitModel();
        ~ArithmeticLogicUnitModel();

        [[nodiscard]] std::string getRenderText() const;

    private:
        uint8_t value;
        bool carry;
        bool zero;

        void resultUpdated(uint8_t newValue, bool newCarryBit, bool newZeroBit) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_ARITHMETICLOGICUNITMODEL_H
