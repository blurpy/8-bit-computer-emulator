#include <bitset>
#include <iostream>

#include "../core/Utils.h"

#include "ValueModel.h"

UI::ValueModel::ValueModel(const std::string &name, const size_t bits) {
    this->name = name;
    this->bits = bits;
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
    return name + ": " + valueAsBinary() + " / " + std::to_string(value);
}

std::string UI::ValueModel::valueAsBinary() const {
    switch (bits) {
        case 4: return std::bitset<4>(value).to_string();
        case 8: return std::bitset<8>(value).to_string();
        default: return "Unhandled";
    }
}
