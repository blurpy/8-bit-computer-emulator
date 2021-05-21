#include <iostream>

#include "Instructions.h"
#include "Utils.h"

#include "InstructionDecoder.h"

Core::InstructionDecoder::InstructionDecoder(const std::shared_ptr<Bus> &bus,
                                             const std::shared_ptr<MemoryAddressRegister> &memoryAddressRegister,
                                             const std::shared_ptr<ProgramCounter> &programCounter,
                                             const std::shared_ptr<RandomAccessMemory> &randomAccessMemory,
                                             const std::shared_ptr<InstructionRegister> &instructionRegister,
                                             const std::shared_ptr<GenericRegister> &aRegister,
                                             const std::shared_ptr<GenericRegister> &bRegister,
                                             const std::shared_ptr<ArithmeticLogicUnit> &arithmeticLogicUnit,
                                             const std::shared_ptr<OutputRegister> &outputRegister,
                                             const std::shared_ptr<FlagsRegister> &flagsRegister,
                                             const std::shared_ptr<Clock> &clock) {
    if (Utils::debugL2()) {
        std::cout << "InstructionDecoder construct" << std::endl;
    }

    this->bus = bus;
    this->memoryAddressRegister = memoryAddressRegister;
    this->programCounter = programCounter;
    this->randomAccessMemory = randomAccessMemory;
    this->instructionRegister = instructionRegister;
    this->aRegister = aRegister;
    this->bRegister = bRegister;
    this->arithmeticLogicUnit = arithmeticLogicUnit;
    this->outputRegister = outputRegister;
    this->flagsRegister = flagsRegister;
    this->clock = clock;
}

Core::InstructionDecoder::~InstructionDecoder() {
    if (Utils::debugL2()) {
        std::cout << "InstructionDecoder destruct" << std::endl;
    }
}

void Core::InstructionDecoder::stepReady(const uint8_t step) {
    if (Utils::debugL2()) {
        std::cout << "InstructionDecoder step received: " << (int) step << std::endl;
    }

    bus->reset(); // The bus is not a register, so it needs to reset when nothing outputs a value

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
            throw std::runtime_error("InstructionDecoder step is unknown: " + std::to_string(step));
    }
}

void Core::InstructionDecoder::handleStep0() const {
    if (Utils::debugL1()) {
        std::cout << "InstructionDecoder step 0 FETCH: MI|CO" << std::endl;
    }

    memoryAddressRegister->in(); // MI
    programCounter->out(); // CO

    notifyObserver({ControlLine::MI, ControlLine::CO});
}

void Core::InstructionDecoder::handleStep1() const {
    if (Utils::debugL1()) {
        std::cout << "InstructionDecoder step 1 FETCH: RO|II|CE" << std::endl;
    }

    randomAccessMemory->out(); // RO
    instructionRegister->in(); // II
    programCounter->enable(); // CE

    notifyObserver({ControlLine::RO, ControlLine::II, ControlLine::CE});
}

void Core::InstructionDecoder::handleStep2() const {
    uint8_t opcode = instructionRegister->getOpcode();
    const std::bitset<4> &opcodeBits = Utils::to4bits(opcode);

    switch (opcode) {
        case Instructions::NOP.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 NOP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::LDA.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 LDA (" << opcodeBits << "): MI|IO" << std::endl;
            }
            memoryAddressRegister->in(); // MI
            instructionRegister->out(); // IO
            notifyObserver({ControlLine::MI, ControlLine::IO});
            break;
        case Instructions::ADD.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 ADD (" << opcodeBits << "): MI|IO" << std::endl;
            }
            memoryAddressRegister->in(); // MI
            instructionRegister->out(); // IO
            notifyObserver({ControlLine::MI, ControlLine::IO});
            break;
        case Instructions::SUB.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 SUB (" << opcodeBits << "): MI|IO" << std::endl;
            }
            memoryAddressRegister->in(); // MI
            instructionRegister->out(); // IO
            notifyObserver({ControlLine::MI, ControlLine::IO});
            break;
        case Instructions::STA.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 STA (" << opcodeBits << "): MI|IO" << std::endl;
            }
            memoryAddressRegister->in(); // MI
            instructionRegister->out(); // IO
            notifyObserver({ControlLine::MI, ControlLine::IO});
            break;
        case Instructions::LDI.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 LDI (" << opcodeBits << "): IO|AI" << std::endl;
            }
            instructionRegister->out(); // IO
            aRegister->in(); // AI
            notifyObserver({ControlLine::IO, ControlLine::AI});
            break;
        case Instructions::JMP.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 JMP (" << opcodeBits << "): IO|CJ" << std::endl;
            }
            instructionRegister->out(); // IO
            programCounter->jump(); // CJ
            notifyObserver({ControlLine::IO, ControlLine::CJ});
            break;
        case Instructions::JC.opcode:
            if (flagsRegister->isCarryFlag()) {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JC (" << opcodeBits << "): IO|CJ" << std::endl;
                }
                instructionRegister->out(); // IO
                programCounter->jump(); // CJ
                notifyObserver({ControlLine::IO, ControlLine::CJ});
            } else {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JC (" << opcodeBits << "): Done" << std::endl;
                }
                // Done
                notifyObserver();
            }
            break;
        case Instructions::JZ.opcode:
            if (flagsRegister->isZeroFlag()) {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JZ (" << opcodeBits << "): IO|CJ" << std::endl;
                }
                instructionRegister->out(); // IO
                programCounter->jump(); // CJ
                notifyObserver({ControlLine::IO, ControlLine::CJ});
            } else {
                if (Utils::debugL1()) {
                    std::cout << "InstructionDecoder step 2 JZ (" << opcodeBits << "): Done" << std::endl;
                }
                // Done
                notifyObserver();
            }
            break;
        case Instructions::OUT.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 OUT (" << opcodeBits << "): AO|OI" << std::endl;
            }
            aRegister->out(); // AO
            outputRegister->in(); // OI
            notifyObserver({ControlLine::AO, ControlLine::OI});
            break;
        case Instructions::HLT.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 2 HLT (" << opcodeBits << "): HLT" << std::endl;
            }
            clock->halt(); // HLT
            notifyObserver({ControlLine::HLT});
            break;
        default:
            throw std::runtime_error("InstructionDecoder step 2: unknown opcode " + opcodeBits.to_string());
    }
}

void Core::InstructionDecoder::handleStep3() const {
    uint8_t opcode = instructionRegister->getOpcode();
    const std::bitset<4> &opcodeBits = Utils::to4bits(opcode);

    switch (opcode) {
        case Instructions::NOP.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 NOP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::LDA.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 LDA (" << opcodeBits << "): RO|AI" << std::endl;
            }
            randomAccessMemory->out(); // RO
            aRegister->in(); // AI
            notifyObserver({ControlLine::RO, ControlLine::AI});
            break;
        case Instructions::ADD.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 ADD (" << opcodeBits << "): RO|BI" << std::endl;
            }
            randomAccessMemory->out(); // RO
            bRegister->in(); // BI
            notifyObserver({ControlLine::RO, ControlLine::BI});
            break;
        case Instructions::SUB.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 SUB (" << opcodeBits << "): RO|BI" << std::endl;
            }
            randomAccessMemory->out(); // RO
            bRegister->in(); // BI
            notifyObserver({ControlLine::RO, ControlLine::BI});
            break;
        case Instructions::STA.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 STA (" << opcodeBits << "): RI|AO" << std::endl;
            }
            randomAccessMemory->in(); // RI
            aRegister->out(); // AO
            notifyObserver({ControlLine::RI, ControlLine::AO});
            break;
        case Instructions::LDI.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 LDI (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::JMP.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 JMP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::JC.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 JC (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::JZ.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 JZ (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::OUT.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 3 OUT (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        default:
            throw std::runtime_error("InstructionDecoder step 3: unknown opcode " + opcodeBits.to_string());
    }
}

void Core::InstructionDecoder::handleStep4() const {
    uint8_t opcode = instructionRegister->getOpcode();
    const std::bitset<4> &opcodeBits = Utils::to4bits(opcode);

    switch (opcode) {
        case Instructions::NOP.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 NOP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::LDA.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 LDA (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::ADD.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 ADD (" << opcodeBits << "): AI|SO|FI" << std::endl;
            }
            aRegister->in(); // AI
            arithmeticLogicUnit->out(); // SO
            flagsRegister->in(); // FI
            notifyObserver({ControlLine::AI, ControlLine::SO, ControlLine::FI});
            break;
        case Instructions::SUB.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 SUB (" << opcodeBits << "): AI|S-|SO|FI" << std::endl;
            }
            aRegister->in(); // AI
            arithmeticLogicUnit->subtract(); // S-
            arithmeticLogicUnit->out(); // SO
            flagsRegister->in(); // FI
            notifyObserver({ControlLine::AI, ControlLine::SM, ControlLine::SO, ControlLine::FI});
            break;
        case Instructions::STA.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 STA (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::LDI.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 LDI (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::JMP.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 JMP (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::JC.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 JC (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::JZ.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 JZ (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        case Instructions::OUT.opcode:
            if (Utils::debugL1()) {
                std::cout << "InstructionDecoder step 4 OUT (" << opcodeBits << "): Done" << std::endl;
            }
            // Done
            notifyObserver();
            break;
        default:
            throw std::runtime_error("InstructionDecoder step 4: unknown opcode " + opcodeBits.to_string());
    }
}

void Core::InstructionDecoder::notifyObserver(const std::vector<ControlLine> &lines) const {
    if (observer != nullptr) {
        observer->controlWordUpdated(lines);
    }
}

void Core::InstructionDecoder::setObserver(const std::shared_ptr<InstructionDecoderObserver> &newObserver) {
    observer = newObserver;
}
