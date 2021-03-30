#ifndef INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H
#define INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H

#include <bitset>
#include <string>
#include <vector>

class Assembler {

public:
    Assembler();
    ~Assembler();

    struct Instruction {
        std::bitset<4> address;
        std::bitset<4> opcode;
        std::bitset<4> operand;
    };

    std::vector<Instruction> loadInstructions(const std::string &fileName);

private:
    int currentMemoryLocation;

    std::vector<std::string> loadFile(const std::string &fileName);
    std::vector<Instruction> interpret(const std::vector<std::string> &lines);
    std::bitset<4> interpretOpcode(const std::string &opcode);
    std::bitset<4> interpretOperand(const std::string &opcode, std::vector<std::string> &tokens);
    void addInstruction(std::vector<Instruction> &instructions, const std::string &opcode, std::vector<std::string> &tokens);
    void addData(std::vector<Instruction> &instructions, std::vector<std::string> &tokens) const;
    std::vector<std::string> tokenize(const std::string &line) const;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_ASSEMBLER_H
