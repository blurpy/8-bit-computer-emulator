#ifndef INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONS_H
#define INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONS_H

#include <bitset>
#include <map>
#include <string>

class Instructions {

public:
    struct Instruction {
        std::string_view mnemonic;
        uint8_t opcode;
        bool hasOperand;

        [[nodiscard]] std::bitset<4> opcodeAsBitset() const {
            return std::bitset<4>(opcode);
        }

        constexpr bool operator==(Instruction id) const { return opcode == id.opcode && mnemonic == id.mnemonic; }
        constexpr bool operator!=(Instruction id) const { return opcode == id.opcode && mnemonic == id.mnemonic; }
    };

    static constexpr Instruction NOP = {"NOP", 0b0000, false};
    static constexpr Instruction LDA = {"LDA", 0b0001, true};
    static constexpr Instruction ADD = {"ADD", 0b0010, true};
    static constexpr Instruction SUB = {"SUB", 0b0011, true};
    static constexpr Instruction STA = {"STA", 0b0100, true};
    static constexpr Instruction LDI = {"LDI", 0b0101, true};
    static constexpr Instruction JMP = {"JMP", 0b0110, true};
    static constexpr Instruction JC  = {"JC",  0b0111, true};
    static constexpr Instruction JZ  = {"JZ",  0b1000, true};
    static constexpr Instruction OUT = {"OUT", 0b1110, false};
    static constexpr Instruction HLT = {"HLT", 0b1111, false};
    static constexpr Instruction UNKNOWN = {"UNKNOWN", 0, false};

    static Instruction find(const std::string& mnemonic);
    static std::bitset<4> noOperand();

private:
    static constexpr std::array<Instruction, 11> ALL = {NOP, LDA, ADD, SUB, STA, LDI, JMP, JC, JZ, OUT, HLT};
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_INSTRUCTIONS_H
