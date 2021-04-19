#ifndef INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H
#define INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H

#include <bitset>
#include <string>
#include <vector>

namespace Core {

    /**
     * Reads files of type .asm with assembly source code, and turns the lines into machine instructions
     * that can be programmed into memory. The machine instructions map to 8-bit values, in a 4-bit address space.
     * That gives at most 16 bytes of instructions and data.
     *
     * Supports all the official instructions, as well as 2 pseudo-instructions that can be used for adding
     * data to the memory before the program runs:
     *
     * - ORG: "Origin" - changes memory location to the address in the parameter
     * - DB: "Define byte" - sets the parameter as a byte in memory at the current memory location
     *
     * Comments in asm source files start with ';'
     */
    class Assembler {

    public:
        Assembler();
        ~Assembler();

        struct Instruction {
            std::bitset<4> address;
            std::bitset<4> opcode;
            std::bitset<4> operand;
        };

        /** Turns the assembly code in the file into machine instructions. */
        std::vector<Instruction> loadInstructions(const std::string &fileName);

    private:
        uint8_t currentMemoryLocation;

        std::vector<std::string> loadFile(const std::string &fileName);
        std::vector<Instruction> interpret(const std::vector<std::string> &lines);
        std::bitset<4> interpretMnemonic(const std::string &mnemonic);
        std::bitset<4> interpretOperand(const std::string &mnemonic, const std::vector<std::string> &tokens);
        void addInstruction(std::vector<Instruction> &instructions, const std::string &mnemonic, const std::vector<std::string> &tokens);
        void addData(std::vector<Instruction> &instructions, const std::vector<std::string> &tokens) const;
        [[nodiscard]] std::vector<std::string> tokenize(const std::string &line) const;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H
