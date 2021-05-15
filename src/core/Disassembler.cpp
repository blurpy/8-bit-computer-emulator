#include "Instructions.h"

#include "Disassembler.h"

std::string Core::Disassembler::disassemble(const uint8_t instruction) {
    uint8_t opcode = instruction >> 4; // First 4 bits
    uint8_t operand = instruction & 0x0F; // Last 4 bits

    switch (opcode) {
        case Instructions::NOP.opcode: return std::string(Instructions::NOP.mnemonic);
        case Instructions::LDA.opcode: return std::string(Instructions::LDA.mnemonic) + " " + std::to_string(operand);
        case Instructions::ADD.opcode: return std::string(Instructions::ADD.mnemonic) + " " + std::to_string(operand);
        case Instructions::SUB.opcode: return std::string(Instructions::SUB.mnemonic) + " " + std::to_string(operand);
        case Instructions::STA.opcode: return std::string(Instructions::STA.mnemonic) + " " + std::to_string(operand);
        case Instructions::LDI.opcode: return std::string(Instructions::LDI.mnemonic) + " " + std::to_string(operand);
        case Instructions::JMP.opcode: return std::string(Instructions::JMP.mnemonic) + " " + std::to_string(operand);
        case Instructions::JC.opcode: return std::string(Instructions::JC.mnemonic) + " " + std::to_string(operand);
        case Instructions::JZ.opcode: return std::string(Instructions::JZ.mnemonic) + " " + std::to_string(operand);
        case Instructions::OUT.opcode: return std::string(Instructions::OUT.mnemonic);
        case Instructions::HLT.opcode: return std::string(Instructions::HLT.mnemonic);
        default: return "UNKNOWN";
    }
}
