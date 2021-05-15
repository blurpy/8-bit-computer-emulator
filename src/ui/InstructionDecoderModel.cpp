#include <iostream>

#include "../core/Utils.h"

#include "InstructionDecoderModel.h"

UI::InstructionDecoderModel::InstructionDecoderModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "InstructionDecoderModel construct" << std::endl;
    }
}

UI::InstructionDecoderModel::~InstructionDecoderModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "InstructionDecoderModel destruct" << std::endl;
    }
}

void UI::InstructionDecoderModel::controlWordUpdated(const std::vector<Core::ControlLine> &newLines) {
    for (auto [key, value] : lines) {
        lines[key] = false;
    }

    for (Core::ControlLine line : newLines) {
        lines[line] = true;
    }
}

std::string UI::InstructionDecoderModel::getRenderTitleText() const {
    return "HLT MI RI RO II IO AI AO BI BO S- SO OI O- CE CO CJ FI";
}

std::string UI::InstructionDecoderModel::getRenderValueText() const {
    return " " +  std::to_string(lines.at(Core::ControlLine::HLT)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::MI)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::RI)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::RO)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::II)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::IO)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::AI)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::AO)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::BI)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::BO)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::SM)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::SO)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::OI)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::OM)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::CE)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::CO)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::CJ)) +
            "  " +  std::to_string(lines.at(Core::ControlLine::FI));
}
