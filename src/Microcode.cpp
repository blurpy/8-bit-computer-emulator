#include <cassert>
#include <iostream>
#include <utility>

#include "Microcode.h"

Microcode::Microcode(std::shared_ptr<MemoryAddressRegister> mar, std::shared_ptr<ProgramCounter> pc,
                     std::shared_ptr<RandomAccessMemory> ram, std::shared_ptr<InstructionRegister> instructionRegister,
                     std::shared_ptr<Register> aRegister, std::shared_ptr<Register> bRegister,
                     std::shared_ptr<ArithmeticLogicUnit> alu, std::shared_ptr<OutputRegister> out,
                     std::shared_ptr<Clock> clock) {
    std::cout << "Microcode in" << std::endl;

    this->mar = std::move(mar);
    this->pc = std::move(pc);
    this->ram = std::move(ram);
    this->instructionRegister = std::move(instructionRegister);
    this->aRegister = std::move(aRegister);
    this->bRegister = std::move(bRegister);
    this->alu = std::move(alu);
    this->out = std::move(out);
    this->clock = std::move(clock);
}

Microcode::~Microcode() {
    std::cout << "Microcode out" << std::endl;
}

void Microcode::stepReady(uint8_t step) {
    std::cout << "Microcode step received: " << (int) step << std::endl;

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
            assert(false);
    }
}

void Microcode::handleStep0() const {
    std::cout << "Microcode step 0 FETCH" << std::endl;

    pc->out(); // CO
    mar->in(); // MI
}

void Microcode::handleStep1() const {
    std::cout << "Microcode step 1 FETCH" << std::endl;

    ram->out(); // RO
    instructionRegister->in(); // II
    pc->enable(); // CE
}

void Microcode::handleStep2() const {
    uint8_t opcode = instructionRegister->getOpcode();
    std::cout << "Microcode step 2 for opcode " << (int) opcode << std::endl;

    switch (opcode) {
        case 0x0001:
            std::cout << "Microcode step 2 LDA" << std::endl;
            mar->in(); // MI
            instructionRegister->out(); // IO
            break;
        case 0b0010:
            std::cout << "Microcode step 2 ADD" << std::endl;
            mar->in(); // MI
            instructionRegister->out(); // IO
            break;
        case 0b0100:
            std::cout << "Microcode step 2 STA" << std::endl;
            mar->in(); // MI
            instructionRegister->out(); // IO
            break;
        case 0b0101:
            std::cout << "Microcode step 2 LDI" << std::endl;
            instructionRegister->out(); // IO
            aRegister->in(); // AI
            break;
        case 0b0110:
            std::cout << "Microcode step 2 JMP" << std::endl;
            instructionRegister->out(); // IO
            pc->jump(); // CJ
            break;
        case 0b1110:
            std::cout << "Microcode step 2 OUT" << std::endl;
            aRegister->out(); // AO
            out->in(); // OI
            break;
        case 0b1111:
            std::cout << "Microcode step 2 HLT" << std::endl;
            clock->stop(); // HLT
            break;
        default:
            assert(false);
    }
}

void Microcode::handleStep3() const {
    uint8_t opcode = instructionRegister->getOpcode();
    std::cout << "Microcode step 3 for opcode " << (int) opcode << std::endl;

    switch (opcode) {
        case 0x0001:
            std::cout << "Microcode step 3 LDA" << std::endl;
            ram->out(); // RO
            aRegister->in(); // AI
            break;
        case 0b0010:
            std::cout << "Microcode step 3 ADD" << std::endl;
            ram->out(); // RO
            bRegister->in(); // BI
            break;
        case 0b0100:
            std::cout << "Microcode step 3 STA" << std::endl;
            ram->in(); // RI
            aRegister->out(); // AO
            break;
        case 0b0101:
            std::cout << "Microcode step 3 LDI" << std::endl;
            // Done
            break;
        case 0b0110:
            std::cout << "Microcode step 3 JMP" << std::endl;
            // Done
            break;
        case 0b1110:
            std::cout << "Microcode step 3 OUT" << std::endl;
            // Done
            break;
        default:
            assert(false);
    }
}

void Microcode::handleStep4() const {
    uint8_t opcode = instructionRegister->getOpcode();
    std::cout << "Microcode step 4 for opcode " << (int) opcode << std::endl;

    switch (opcode) {
        case 0x0001:
            std::cout << "Microcode step 4 LDA" << std::endl;
            // Done
            break;
        case 0b0010:
            std::cout << "Microcode step 4 ADD" << std::endl;
            aRegister->in(); // AI
            alu->out(); // SO
            // TODO FI
            break;
        case 0b0100:
            std::cout << "Microcode step 4 STA" << std::endl;
            // Done
            break;
        case 0b0101:
            std::cout << "Microcode step 4 LDI" << std::endl;
            // Done
            break;
        case 0b0110:
            std::cout << "Microcode step 4 JMP" << std::endl;
            // Done
            break;
        case 0b1110:
            std::cout << "Microcode step 4 OUT" << std::endl;
            // Done
            break;
        default:
            assert(false);
    }
}
