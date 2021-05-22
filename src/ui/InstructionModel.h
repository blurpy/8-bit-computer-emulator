#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONMODEL_H

#include <memory>
#include <string>

#include "ValueModel.h"

namespace UI {

    /**
     * Infers which instruction is being executed at the moment based on the state of the
     * core step counter and instruction register for presentation in the user interface.
     */
    class InstructionModel {

    public:
        InstructionModel(const std::shared_ptr<ValueModel> &stepCounter,
                         const std::shared_ptr<ValueModel> &instructionRegister);
        ~InstructionModel();

        [[nodiscard]] std::string getRenderText() const;

    private:
        std::shared_ptr<ValueModel> stepCounter;
        std::shared_ptr<ValueModel> instructionRegister;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONMODEL_H
