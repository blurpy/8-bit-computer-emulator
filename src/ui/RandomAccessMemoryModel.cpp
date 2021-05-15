#include <bitset>
#include <iostream>

#include "../core/Utils.h"

#include "RandomAccessMemoryModel.h"

UI::RandomAccessMemoryModel::RandomAccessMemoryModel(const std::shared_ptr<ValueModel> &memoryAddressRegister) {
    if (Core::Utils::debugL2()) {
        std::cout << "RandomAccessMemoryModel construct" << std::endl;
    }

    this->memoryAddressRegister = memoryAddressRegister;
    this->value = 0;
}

UI::RandomAccessMemoryModel::~RandomAccessMemoryModel() {
    if (Core::Utils::debugL2()) {
        std::cout << "RandomAccessMemoryModel destruct" << std::endl;
    }
}

void UI::RandomAccessMemoryModel::valueUpdated(const uint8_t newValue) {
    value = newValue;
    memory[memoryAddressRegister->getValue()] = newValue;
}

std::string UI::RandomAccessMemoryModel::getRenderText() const {
    return "Random Access Memory: " + std::bitset<8>(value).to_string() + " / " + std::to_string(value);
}

std::array<std::string, UI::RandomAccessMemoryModel::MEMORY_SIZE> UI::RandomAccessMemoryModel::getRenderTextFull() const {
    std::array<std::string, MEMORY_SIZE> text{};

    for (int i = 0; i < memory.size(); i++) {
        const uint8_t currentValue = memory[i];
        text[i] = std::bitset<8>(currentValue).to_string() + " / " + std::to_string(currentValue);
    }

    return text;
}
