#include <cassert>
#include <iostream>
#include <utility>

#include "Utils.h"

#include "InstructionDecoder.h"

InstructionDecoder::InstructionDecoder(std::shared_ptr<MemoryAddressRegister> mar, std::shared_ptr<ProgramCounter> pc,
                                       std::shared_ptr<RandomAccessMemory> ram, std::shared_ptr<InstructionRegister> instructionRegister,
                                       std::shared_ptr<Register> aRegister, std::shared_ptr<Register> bRegister,
                                       std::shared_ptr<ArithmeticLogicUnit> alu, std::shared_ptr<OutputRegister> out,
                                       std::shared_ptr<FlagsRegister> flagsRegister, std::shared_ptr<Clock> clock) {
    if (Utils::debugL2()) {
        std::cout << "InstructionDecoder in" << std::endl;
    }

    this->mar = std::move(mar);
    this->pc = std::move(pc);
    this->ram = std::move(ram);
    this->instructionRegister = std::move(instructionRegister);
    this->aRegister = std::move(aRegister);
    this->bRegister = std::move(bRegister);
    this->alu = std::move(alu);
    this->out = std::move(out);
    this->flagsRegister = std::move(flagsRegister);
    this->clock = std::move(clock);
}

InstructionDecoder::~InstructionDecoder() {
    if (Utils::debugL2()) {
        std::cout << "InstructionDecoder out" << std::endl;
    }
}

void InstructionDecoder::stepReady(uint8_t step) {
    if (Utils::debugL2()) {
        std::cout << "InstructionDecoder step received: " << (int) step << std::endl;
    }

    switch (step) {
        case 0:
            handleStep0();
            break;
        case 1:
            handleStep1();
            break;
        case 2:
            handleStep2();
            break;
        case 3:
            handleStep3();
            break;
        case 4:
            handleStep4();
            break;
        default:
            std::cerr << "InstructionDecoder step is unknown: " << (int) step << std::endl;
            assert(false);
    }
}

void InstructionDecoder::handleStep0() const {
    if (Utils::debugL1()) {
        std::cout << "InstructionDecoder step 0 FETCH: CO|MI" << std::endl;
    }

    pc->out(); // CO
    mar->in(); // MI
}

void InstructionDecoder::handleStep1() const {
    if (Utils::debugL1()) {
        std::cout << "InstructionDecoder step 1 FETCH: RO|II|CE" << std::endl;
    }

    ram->out(); // RO
    instructionRegister->in(); // II
    pc->enable(); // CE
}

void InstructionDecoder::handleStep2() const {
    uint8_t opcode = instructionRegister->getOpcode();
    const std::bitset<4> &opcodeBits = Utils::to4bits(opcode);

    switch (opcode) {
        case 0x0000:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 NOP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0x0001:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 LDA (" << opcodeBits << "): MI|IO" << std::endl;
            }
            mar->in(); // MI
            instructionRegister->out(); // IO
            break;
        case 0b0010:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 ADD (" << opcodeBits << "): MI|IO" << std::endl;
            }
            mar->in(); // MI
            instructionRegister->out(); // IO
            break;
        case 0b0011:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 SUB (" << opcodeBits << "): MI|IO" << std::endl;
            }
            mar->in(); // MI
            instructionRegister->out(); // IO
            break;
        case 0b0100:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 STA (" << opcodeBits << "): MI|IO" << std::endl;
            }
            mar->in(); // MI
            instructionRegister->out(); // IO
            break;
        case 0b0101:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 LDI (" << opcodeBits << "): IO|AI" << std::endl;
            }
            instructionRegister->out(); // IO
            aRegister->in(); // AI
            break;
        case 0b0110:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 JMP (" << opcodeBits << "): IO|CJ" << std::endl;
            }
            instructionRegister->out(); // IO
            pc->jump(); // CJ
            break;
        case 0b0111:
            if (flagsRegister->isCarryFlag()) {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JC (" << opcodeBits << "): IO|CJ" << std::endl;
                }
                instructionRegister->out(); // IO
                pc->jump(); // CJ
            } else {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JC (" << opcodeBits << "): Done" << std::endl;
                }
                // Done
            }
            break;
        case 0b1000:
            if (flagsRegister->isZeroFlag()) {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JZ (" << opcodeBits << "): IO|CJ" << std::endl;
                }
                instructionRegister->out(); // IO
                pc->jump(); // CJ
            } else {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JZ (" << opcodeBits << "): Done" << std::endl;
                }
                // Done
            }
            break;
        case 0b1110:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 OUT (" << opcodeBits << "): AO|OI" << std::endl;
            }
            aRegister->out(); // AO
            out->in(); // OI
            break;
        case 0b1111:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 HLT (" << opcodeBits << "): HLT" << std::endl;
            }
            clock->stop(); // HLT
            break;
        default:
            std::cerr << "InstructionDecoder step 2: unknown opcode " << opcodeBits << std::endl;
            assert(false);
    }
}

void InstructionDecoder::handleStep3() const {
    uint8_t opcode = instructionRegister->getOpcode();
    const std::bitset<4> &opcodeBits = Utils::to4bits(opcode);

    switch (opcode) {
        case 0x0000:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 NOP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0x0001:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 LDA (" << opcodeBits << "): RO|AI" << std::endl;
            }
            ram->out(); // RO
            aRegister->in(); // AI
            break;
        case 0b0010:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 ADD (" << opcodeBits << "): RO|BI" << std::endl;
            }
            ram->out(); // RO
            bRegister->in(); // BI
            break;
        case 0b0011:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 SUB (" << opcodeBits << "): RO|BI" << std::endl;
            }
            ram->out(); // RO
            bRegister->in(); // BI
            break;
        case 0b0100:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 STA (" << opcodeBits << "): RI|AO" << std::endl;
            }
            ram->in(); // RI
            aRegister->out(); // AO
            break;
        case 0b0101:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 LDI (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b0110:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 JMP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b0111:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 JC (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b1000:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 JZ (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b1110:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 OUT (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        default:
            std::cerr << "InstructionDecoder step 3: unknown opcode " << opcodeBits << std::endl;
            assert(false);
    }
}

void InstructionDecoder::handleStep4() const {
    uint8_t opcode = instructionRegister->getOpcode();
    const std::bitset<4> &opcodeBits = Utils::to4bits(opcode);

    switch (opcode) {
        case 0x0000:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 NOP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0x0001:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 LDA (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b0010:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 ADD (" << opcodeBits << "): AI|SO|FI" << std::endl;
            }
            aRegister->in(); // AI
            alu->out(); // SO
            flagsRegister->in(); // FI
            break;
        case 0b0011:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 SUB (" << opcodeBits << "): AI|S-|SO|FI" << std::endl;
            }
            aRegister->in(); // AI
            alu->subtract(); // S-
            alu->out(); // SO
            flagsRegister->in(); // FI
            break;
        case 0b0100:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 STA (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b0101:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 LDI (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b0110:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 JMP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b0111:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 JC (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b1000:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 JZ (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        case 0b1110:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 OUT (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            break;
        default:
            std::cerr << "InstructionDecoder step 4: unknown opcode " << opcodeBits << std::endl;
            assert(false);
    }
}
