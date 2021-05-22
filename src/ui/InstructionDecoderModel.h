#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODERMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODERMODEL_H

#include <map>
#include <string>

#include "../core/InstructionDecoderObserver.h"

namespace UI {

    /**
     * Observes the core instruction decoder and prepares the state for presentation in the user interface.
     */
    class InstructionDecoderModel : public Core::InstructionDecoderObserver {

    public:
        InstructionDecoderModel();
        ~InstructionDecoderModel();

        [[nodiscard]] std::string getRenderTitleText() const;
        [[nodiscard]] std::string getRenderValueText() const;

    private:
        std::map<Core::ControlLine, bool> lines {
                {Core::ControlLine::HLT, false},
                {Core::ControlLine::MI, false},
                {Core::ControlLine::RI, false},
                {Core::ControlLine::RO, false},
                {Core::ControlLine::II, false},
                {Core::ControlLine::IO, false},
                {Core::ControlLine::AI, false},
                {Core::ControlLine::AO, false},
                {Core::ControlLine::BI, false},
                {Core::ControlLine::BO, false},
                {Core::ControlLine::SM, false},
                {Core::ControlLine::SO, false},
                {Core::ControlLine::OI, false},
                {Core::ControlLine::OM, false},
                {Core::ControlLine::CE, false},
                {Core::ControlLine::CO, false},
                {Core::ControlLine::CJ, false},
                {Core::ControlLine::FI, false},
        };

        void controlWordUpdated(const std::vector<Core::ControlLine> &newLines) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODERMODEL_H
