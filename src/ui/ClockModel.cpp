#include <iomanip>
#include <iostream>
#include <sstream> // Needed to build on macos and windows

#include "../core/Utils.h"

#include "ClockModel.h"

UI::ClockModel::ClockModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "ClockModel construct" << std::endl;
    }

    this->on = false;
    this->frequency = 0;
}

UI::ClockModel::~ClockModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "ClockModel destruct" << std::endl;
    }
}

void UI::ClockModel::clockTicked(const bool newOn) {
    on = newOn;
}

void UI::ClockModel::frequencyChanged(const double newHz) {
    frequency = newHz;
}

std::string UI::ClockModel::getRenderText() const {
    std::stringstream hzStream;
    hzStream << std::fixed << std::setprecision(1) << frequency;

    return "Clock: " + std::to_string(on) + " / " + hzStream.str() + " Hz";
}
