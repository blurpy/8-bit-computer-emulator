#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Assembler.h"

Assembler::Assembler() {
    std::cout << "Assembler in" << std::endl;
    this->currentMemoryLocation = 0;
}

Assembler::~Assembler() {
    std::cout << "Assembler out" << std::endl;
}

std::vector<Assembler::Instruction> Assembler::loadInstructions(const std::string &fileName) {
    std::vector<std::string> lines = loadFile(fileName);
    std::vector<Instruction> instructions = interpret(lines);

    return instructions;
}

std::vector<std::string> Assembler::loadFile(const std::string &fileName) {
    std::cout << "Assembler: loading file: " << fileName << std::endl;

    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Assembler: failed to open file: " << fileName << std::endl;
        return std::vector<std::string>();
    }

    std::vector<std::string> lines;
    std::string currentLine;

    while (std::getline(file, currentLine)) {
        if (!currentLine.empty()) {
            lines.push_back(currentLine);
        }
    }

    file.close();

    return lines;
}

std::vector<Assembler::Instruction> Assembler::interpret(const std::vector<std::string> &lines) {
    std::vector<Instruction> instructions;

    for (const auto &line : lines) {
        std::cout << "Assembler: " << line << std::endl;

        std::vector<std::string> tokens = tokenize(line);
        std::string opcode = tokens[0];

        // Supports 2 pseudo-instructions that can be used for adding data to the memory before the program runs.
        // This is to support the flexibility of the DIP switches in the memory module.
        if (opcode == "ORG") {
            // "Origin" - changes memory location to the address in the parameter
            std::string operand = tokens[1];
            currentMemoryLocation = std::stoi(operand);
        } else if (opcode == "DB") {
            // "Define byte" - sets the parameter as a byte in memory at the current memory location
            addData(instructions, tokens);
            currentMemoryLocation++;
        } else {
            addInstruction(instructions, opcode, tokens);
            currentMemoryLocation++;
        }
    }

    return instructions;
}

void Assembler::addData(std::vector<Instruction> &instructions, std::vector<std::string> &tokens) const {
    assert(tokens.size() == 2);

    uint8_t value = std::stoi(tokens[1]);
    std::bitset<4> msb = value >> 4;
    std::bitset<4> lsb = value & 0x0F;

    Assembler::Instruction instruction = {std::bitset<4>(currentMemoryLocation), msb, lsb};
    instructions.push_back(instruction);

    std::cout << "Assembler: " << instruction.address << " " << instruction.opcode << " " << instruction.operand << std::endl;
}

void Assembler::addInstruction(std::vector<Instruction> &instructions, const std::string &opcode, std::vector<std::string> &tokens) {
    const std::bitset<4> &opcodeBitset = interpretOpcode(opcode);
    const std::bitset<4> &operandBitset = interpretOperand(opcode, tokens);

    Assembler::Instruction instruction = {std::bitset<4>(currentMemoryLocation), opcodeBitset, operandBitset};
    instructions.push_back(instruction);

    std::cout << "Assembler: " << instruction.address << " " << instruction.opcode << " " << instruction.operand << std::endl;
}

std::bitset<4> Assembler::interpretOpcode(const std::string &opcode) {
    if (opcode == "LDA") {
        return std::bitset<4>("0001");
    } else if (opcode == "ADD") {
        return std::bitset<4>("0010");
    } else if (opcode == "STA") {
        return std::bitset<4>("0100");
    } else if (opcode == "LDI") {
        return std::bitset<4>("0101");
    } else if (opcode == "JMP") {
        return std::bitset<4>("0110");
    } else if (opcode == "OUT") {
        return std::bitset<4>("1110");
    } else if (opcode == "HLT") {
        return std::bitset<4>("1111");
    }

    assert(false);
}

std::bitset<4> Assembler::interpretOperand(const std::string &opcode, std::vector<std::string> &tokens) {
    if (opcode == "LDA") {
        assert(tokens.size() == 2);
        return std::bitset<4>(std::stoi(tokens[1]));
    } else if (opcode == "ADD") {
        assert(tokens.size() == 2);
        return std::bitset<4>(std::stoi(tokens[1]));
    } else if (opcode == "STA") {
        assert(tokens.size() == 2);
        return std::bitset<4>(std::stoi(tokens[1]));
    } else if (opcode == "LDI") {
        assert(tokens.size() == 2);
        return std::bitset<4>(std::stoi(tokens[1]));
    } else if (opcode == "JMP") {
        assert(tokens.size() == 2);
        return std::bitset<4>(std::stoi(tokens[1]));
    } else if (opcode == "OUT") {
        assert(tokens.size() == 1);
        return std::bitset<4>("0000");
    } else if (opcode == "HLT") {
        assert(tokens.size() == 1);
        return std::bitset<4>("0000");
    }

    assert(false);
}

std::vector<std::string> Assembler::tokenize(const std::string &line) const {
    std::stringstream stream(line);
    std::vector<std::string> tokens;
    std::string token;

    while (stream >> token) {
        // Drop comments
        if (token == ";") { // TODO starts with?
            break;
        }

        tokens.push_back(token);
    }

    return tokens;
}
