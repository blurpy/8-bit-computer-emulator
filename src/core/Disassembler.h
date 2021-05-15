#ifndef INC_8_BIT_COMPUTER_EMULATOR_DISASSEMBLER_H
#define INC_8_BIT_COMPUTER_EMULATOR_DISASSEMBLER_H

#include <cstdint>
#include <string>

namespace Core {

    /**
     * Static class for disassembling instructions, as in converting binary code into assembly again.
     *
     * Example: 0011 1001 -> SUB 9
     */
    class Disassembler {

    public:
        Disassembler() = delete;
        ~Disassembler() = delete;

        /** Converts a binary instruction into assembly. */
        static std::string disassemble(uint8_t instruction);
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_DISASSEMBLER_H
