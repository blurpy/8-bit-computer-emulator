#ifndef INC_8_BIT_COMPUTER_EMULATOR_VALUEMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_VALUEMODEL_H

#include <string>

#include "../core/ValueObserver.h"

namespace UI {

    class ValueModel: public Core::ValueObserver {

    public:
        ValueModel(const std::string &name, size_t bits);
        ~ValueModel();

        [[nodiscard]] std::string getRenderText() const;

    private:
        std::string name;
        size_t bits;
        uint8_t value;

        [[nodiscard]] std::string valueAsBinary() const;

        void valueUpdated(uint8_t newValue) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_VALUEMODEL_H
