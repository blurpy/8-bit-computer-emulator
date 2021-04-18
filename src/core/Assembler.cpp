#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Instructions.h"
#include "Utils.h"

#include "Assembler.h"

Core::Assembler::Assembler() {
    if (Utils::debugL2()) {
        std::cout << "Assembler construct" << std::endl;
    }

    this->currentMemoryLocation = 0;
}

Core::Assembler::~Assembler() {
    if (Utils::debugL2()) {
        std::cout << "Assembler destruct" << std::endl;
    }
}

std::vector<Core::Assembler::Instruction> Core::Assembler::loadInstructions(const std::string &fileName) {
    std::vector<std::string> lines = loadFile(fileName);
    std::vector<Instruction> instructions = interpret(lines);

    return instructions;
}

std::vector<std::string> Core::Assembler::loadFile(const std::string &fileName) {
    std::cout << "Assembler: loading file: " << fileName << std::endl;

    std::ifstream file(fileName);

    if (!file.is_open()) {
        throw std::runtime_error("Assembler: failed to open file: " + fileName);
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

std::vector<Core::Assembler::Instruction> Core::Assembler::interpret(const std::vector<std::string> &lines) {
    std::vector<Instruction> instructions;

    for (const auto &line : lines) {
        if (Utils::debugL1()) {
            std::cout << "Assembler: " << line << std::endl;
        }

        if (currentMemoryLocation > Utils::FOUR_BITS_MAX) {
            throw std::runtime_error("Assembler: address out of bounds " + std::to_string(currentMemoryLocation));
        }

        std::vector<std::string> tokens = tokenize(line);

        if (tokens.empty()) {
            continue; // Skip pure comment lines
        }

        std::string mnemonic = tokens[0];

        // Supports 2 pseudo-instructions that can be used for adding data to the memory before the program runs.
        // This is to support the flexibility of the DIP switches in the memory module.
        if (mnemonic == "ORG") {
            // "Origin" - changes memory location to the address in the parameter
            std::string operand = tokens[1];
            currentMemoryLocation = std::stoi(operand);
        } else if (mnemonic == "DB") {
            // "Define byte" - sets the parameter as a byte in memory at the current memory location
            addData(instructions, tokens);
            currentMemoryLocation++;
        } else {
            addInstruction(instructions, mnemonic, tokens);
            currentMemoryLocation++;
        }
    }

    return instructions;
}

void Core::Assembler::addData(std::vector<Instruction> &instructions, const std::vector<std::string> &tokens) const {
    if (tokens.size() != 2) {
        throw std::runtime_error("Assembler: wrong number of arguments to data");
    }

    uint8_t value = std::stoi(tokens[1]);
    std::bitset<4> msb = value >> 4;
    std::bitset<4> lsb = value & 0x0F;

    Assembler::Instruction instruction = {std::bitset<4>(currentMemoryLocation), msb, lsb};
    instructions.push_back(instruction);

    if (Utils::debugL1()) {
        std::cout << "Assembler: " << instruction.address << " " << instruction.opcode << " " << instruction.operand
                  << std::endl;
    }
}

void Core::Assembler::addInstruction(std::vector<Instruction> &instructions, const std::string &mnemonic, const std::vector<std::string> &tokens) {
    const std::bitset<4> &opcodeBitset = interpretMnemonic(mnemonic);
    const std::bitset<4> &operandBitset = interpretOperand(mnemonic, tokens);

    Assembler::Instruction instruction = {std::bitset<4>(currentMemoryLocation), opcodeBitset, operandBitset};
    instructions.push_back(instruction);

    if (Utils::debugL1()) {
        std::cout << "Assembler: " << instruction.address << " " << instruction.opcode << " " << instruction.operand
                  << std::endl;
    }
}

std::bitset<4> Core::Assembler::interpretMnemonic(const std::string &mnemonic) {
    const Instructions::Instruction &instruction = Instructions::find(mnemonic);

    if (instruction == Instructions::UNKNOWN) {
        throw std::runtime_error("Assembler: interpret mnemonic - unknown mnemonic " + mnemonic);
    }

    return instruction.opcodeAsBitset();
}

std::bitset<4> Core::Assembler::interpretOperand(const std::string &mnemonic, const std::vector<std::string> &tokens) {
    const Instructions::Instruction &instruction = Instructions::find(mnemonic);

    if (instruction == Instructions::UNKNOWN) {
        throw std::runtime_error("Assembler: interpret operand - unknown mnemonic " + mnemonic);
    }

    if (instruction.hasOperand) {
        if (tokens.size() != 2) {
            throw std::runtime_error("Assembler: interpret operand - wrong number of arguments to " + mnemonic);
        }

        uint8_t operand = std::stoi(tokens[1]);

        if (operand > Utils::FOUR_BITS_MAX) {
            throw std::runtime_error("Assembler: interpret operand - out of bounds " + std::to_string(operand));
        }

        return std::bitset<4>(operand);
    } else {
        return Instructions::noOperand();
    }
}

std::vector<std::string> Core::Assembler::tokenize(const std::string &line) const {
    std::stringstream stream(line);
    std::vector<std::string> tokens;
    std::string token;

    while (stream >> token) {
        // Drop comments
        if (Utils::startsWith(token, ";")) {
            break;
        }

        tokens.push_back(token);

        if (Utils::debugL2()) {
            std::cout << "Token: " << token << std::endl;
        }
    }

    return tokens;
}
