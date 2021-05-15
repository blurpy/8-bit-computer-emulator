#include <doctest.h>

#include <iostream> // Due to bug with doctest on macOS in release mode: https://github.com/onqtam/doctest/issues/126
#include <bitset>

#include "core/Disassembler.h"

using namespace Core;

TEST_SUITE("DisassemblerTest") {
    TEST_CASE("should disassemble NOP") {
        CHECK_EQ("NOP", Disassembler::disassemble(std::bitset<8>("00000000").to_ulong()));
    }

    TEST_CASE("should disassemble LDA") {
        CHECK_EQ("LDA 10", Disassembler::disassemble(std::bitset<8>("00011010").to_ulong()));
    }

    TEST_CASE("should disassemble ADD") {
        CHECK_EQ("ADD 11", Disassembler::disassemble(std::bitset<8>("00101011").to_ulong()));
    }

    TEST_CASE("should disassemble SUB") {
        CHECK_EQ("SUB 12", Disassembler::disassemble(std::bitset<8>("00111100").to_ulong()));
    }

    TEST_CASE("should disassemble STA") {
        CHECK_EQ("STA 13", Disassembler::disassemble(std::bitset<8>("01001101").to_ulong()));
    }

    TEST_CASE("should disassemble LDI") {
        CHECK_EQ("LDI 14", Disassembler::disassemble(std::bitset<8>("01011110").to_ulong()));
    }

    TEST_CASE("should disassemble JMP") {
        CHECK_EQ("JMP 4", Disassembler::disassemble(std::bitset<8>("01100100").to_ulong()));
    }

    TEST_CASE("should disassemble JC") {
        CHECK_EQ("JC 5", Disassembler::disassemble(std::bitset<8>("01110101").to_ulong()));
    }

    TEST_CASE("should disassemble JZ") {
        CHECK_EQ("JZ 6", Disassembler::disassemble(std::bitset<8>("10000110").to_ulong()));
    }

    TEST_CASE("should disassemble OUT") {
        CHECK_EQ("OUT", Disassembler::disassemble(std::bitset<8>("11100000").to_ulong()));
    }

    TEST_CASE("should disassemble HLT") {
        CHECK_EQ("HLT", Disassembler::disassemble(std::bitset<8>("11110000").to_ulong()));
    }

    TEST_CASE("should handle unknown instruction") {
        CHECK_EQ("UNKNOWN", Disassembler::disassemble(std::bitset<8>("10010000").to_ulong()));
    }
}
