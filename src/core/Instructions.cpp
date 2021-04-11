#include <iostream>

#include "Instructions.h"

Core::Instructions::Instruction Core::Instructions::find(const std::string& mnemonic) {
    for (Instruction candidate : Instructions::ALL) {
        if (mnemonic == candidate.mnemonic) {
            return candidate;
        }
    }

    return UNKNOWN;
}

std::bitset<4> Core::Instructions::noOperand() {
    return std::bitset<4>("0000");
}
