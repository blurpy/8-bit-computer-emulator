#include <bitset>
#include <iostream>

#include "../core/Utils.h"

#include "ValueModel.h"

UI::ValueModel::ValueModel(const std::string &name) {
    this->name = name;
    this->value = 0;

    if (Core::Utils::debugL2()) {
        std::cout << this->name << " Model construct" << std::endl;
    }
}

UI::ValueModel::~ValueModel() {
    if (Core::Utils::debugL2()) {
        std::cout << this->name << " Model destruct" << std::endl;
    }
}

void UI::ValueModel::valueUpdated(const uint8_t newValue) {
    value = newValue;
}

std::string UI::ValueModel::getRenderText() const {
    return name + ": " + std::bitset<8>(value).to_string() + " / " + std::to_string(value);
}
