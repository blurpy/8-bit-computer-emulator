#include <iostream>

#include "../core/Utils.h"

#include "ClockModel.h"

UI::ClockModel::ClockModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "ClockModel construct" << std::endl;
    }

    this->on = false;
}

UI::ClockModel::~ClockModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "ClockModel destruct" << std::endl;
    }
}

void UI::ClockModel::clockTicked(const bool newOn) {
    on = newOn;
}

std::string UI::ClockModel::getRenderText() const {
    return "Clock: " + std::to_string(on);
}
