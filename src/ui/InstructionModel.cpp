#include <iostream>

#include "../core/Disassembler.h"
#include "../core/Utils.h"

#include "InstructionModel.h"

UI::InstructionModel::InstructionModel(const std::shared_ptr<ValueModel> &stepCounter,
                                       const std::shared_ptr<ValueModel> &instructionRegister) {
    if (Core::Utils::debugL2()) {
        std::cout << "InstructionModel construct" << std::endl;
    }

    this->stepCounter = stepCounter;
    this->instructionRegister = instructionRegister;
}

UI::InstructionModel::~InstructionModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "InstructionModel destruct" << std::endl;
    }
}

std::string UI::InstructionModel::getRenderText() const {
    if (stepCounter->getValue() < 2) {
        return "Instruction: FETCH";
    }

    return "Instruction: " + Core::Disassembler::disassemble(instructionRegister->getValue());
}
