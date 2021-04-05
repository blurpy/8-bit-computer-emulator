#ifndef INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H
#define INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H

#include <bitset>
#include <string>
#include <vector>

/**
 * Reads files of type .asm with assembly source code, and turns the lines into machine instructions
 * that can be programmed into memory.
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
    int currentMemoryLocation;

    std::vector<std::string> loadFile(const std::string &fileName);
    std::vector<Instruction> interpret(const std::vector<std::string> &lines);
    std::bitset<4> interpretMnemonic(const std::string &mnemonic);
    std::bitset<4> interpretOperand(const std::string &mnemonic, std::vector<std::string> &tokens);
    void addInstruction(std::vector<Instruction> &instructions, const std::string &mnemonic, std::vector<std::string> &tokens);
    void addData(std::vector<Instruction> &instructions, std::vector<std::string> &tokens) const;
    [[nodiscard]] std::vector<std::string> tokenize(const std::string &line) const;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H
