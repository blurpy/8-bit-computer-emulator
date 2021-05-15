#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODEROBSERVER_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODEROBSERVER_H

#include <vector>

namespace Core {

    /** The short name of all the different control lines. */
    enum class ControlLine {
        //                                       S-          O-
        HLT, MI, RI, RO, II, IO, AI, AO, BI, BO, SM, SO, OI, OM, CE, CO, CJ, FI
    };

    /**
     * Interface for external observation of the instruction decoder of the computer.
     */
    class InstructionDecoderObserver {

    public:
        /** The control word is updated to have the following lines enabled. */
        virtual void controlWordUpdated(const std::vector<ControlLine> &newLines) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONDECODEROBSERVER_H
