#include <iostream>

#include "Instructions.h"

Instructions::Instruction Instructions::find(const std::string& mnemonic) {
    for (Instruction candidate : Instructions::ALL) {
        if (mnemonic == candidate.mnemonic) {
            return candidate;
        }
    }

    return UNKNOWN;
}

std::bitset<4> Instructions::noOperand() {
    return std::bitset<4>("0000");
}
