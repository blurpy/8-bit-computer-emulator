#include "include/doctest.h"

#include <Assembler.h>
#include <Instructions.h>

TEST_SUITE("AssemblerTest") {
    TEST_CASE("loadInstructions() should parse HLT instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &hlt = instructions[0];
        CHECK_EQ(hlt.address.to_ulong(), 0);
        CHECK_EQ(hlt.address.to_string(), "0000");
        CHECK_EQ(hlt.opcode.to_ulong(), Instructions::HLT.opcode);
        CHECK_EQ(hlt.opcode.to_string(), "1111");
        CHECK_EQ(hlt.operand.to_ulong(), 0);
        CHECK_EQ(hlt.operand.to_string(), "0000");
    }

    TEST_CASE("loadInstructions() should parse OUT instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &out = instructions[1];
        CHECK_EQ(out.address.to_ulong(), 1);
        CHECK_EQ(out.address.to_string(), "0001");
        CHECK_EQ(out.opcode.to_ulong(), Instructions::OUT.opcode);
        CHECK_EQ(out.opcode.to_string(), "1110");
        CHECK_EQ(out.operand.to_ulong(), 0);
        CHECK_EQ(out.operand.to_string(), "0000");
    }

    TEST_CASE("loadInstructions() should parse JZ instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &jz = instructions[2];
        CHECK_EQ(jz.address.to_ulong(), 2);
        CHECK_EQ(jz.address.to_string(), "0010");
        CHECK_EQ(jz.opcode.to_ulong(), Instructions::JZ.opcode);
        CHECK_EQ(jz.opcode.to_string(), "1000");
        CHECK_EQ(jz.operand.to_ulong(), 5);
        CHECK_EQ(jz.operand.to_string(), "0101");
    }

    TEST_CASE("loadInstructions() should parse JC instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &jc = instructions[3];
        CHECK_EQ(jc.address.to_ulong(), 3);
        CHECK_EQ(jc.address.to_string(), "0011");
        CHECK_EQ(jc.opcode.to_ulong(), Instructions::JC.opcode);
        CHECK_EQ(jc.opcode.to_string(), "0111");
        CHECK_EQ(jc.operand.to_ulong(), 4);
        CHECK_EQ(jc.operand.to_string(), "0100");
    }

    TEST_CASE("loadInstructions() should parse JMP instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &jmp = instructions[4];
        CHECK_EQ(jmp.address.to_ulong(), 4);
        CHECK_EQ(jmp.address.to_string(), "0100");
        CHECK_EQ(jmp.opcode.to_ulong(), Instructions::JMP.opcode);
        CHECK_EQ(jmp.opcode.to_string(), "0110");
        CHECK_EQ(jmp.operand.to_ulong(), 1);
        CHECK_EQ(jmp.operand.to_string(), "0001");
    }

    TEST_CASE("loadInstructions() should parse LDI instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &ldi = instructions[5];
        CHECK_EQ(ldi.address.to_ulong(), 5);
        CHECK_EQ(ldi.address.to_string(), "0101");
        CHECK_EQ(ldi.opcode.to_ulong(), Instructions::LDI.opcode);
        CHECK_EQ(ldi.opcode.to_string(), "0101");
        CHECK_EQ(ldi.operand.to_ulong(), 3);
        CHECK_EQ(ldi.operand.to_string(), "0011");
    }

    TEST_CASE("loadInstructions() should parse STA instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &sta = instructions[6];
        CHECK_EQ(sta.address.to_ulong(), 6);
        CHECK_EQ(sta.address.to_string(), "0110");
        CHECK_EQ(sta.opcode.to_ulong(), Instructions::STA.opcode);
        CHECK_EQ(sta.opcode.to_string(), "0100");
        CHECK_EQ(sta.operand.to_ulong(), 10);
        CHECK_EQ(sta.operand.to_string(), "1010");
    }

    TEST_CASE("loadInstructions() should parse SUB instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &sub = instructions[7];
        CHECK_EQ(sub.address.to_ulong(), 7);
        CHECK_EQ(sub.address.to_string(), "0111");
        CHECK_EQ(sub.opcode.to_ulong(), Instructions::SUB.opcode);
        CHECK_EQ(sub.opcode.to_string(), "0011");
        CHECK_EQ(sub.operand.to_ulong(), 8);
        CHECK_EQ(sub.operand.to_string(), "1000");
    }

    TEST_CASE("loadInstructions() should parse ADD instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &add = instructions[8];
        CHECK_EQ(add.address.to_ulong(), 8);
        CHECK_EQ(add.address.to_string(), "1000");
        CHECK_EQ(add.opcode.to_ulong(), Instructions::ADD.opcode);
        CHECK_EQ(add.opcode.to_string(), "0010");
        CHECK_EQ(add.operand.to_ulong(), 15);
        CHECK_EQ(add.operand.to_string(), "1111");
    }

    TEST_CASE("loadInstructions() should parse LDA instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &lda = instructions[9];
        CHECK_EQ(lda.address.to_ulong(), 9);
        CHECK_EQ(lda.address.to_string(), "1001");
        CHECK_EQ(lda.opcode.to_ulong(), Instructions::LDA.opcode);
        CHECK_EQ(lda.opcode.to_string(), "0001");
        CHECK_EQ(lda.operand.to_ulong(), 14);
        CHECK_EQ(lda.operand.to_string(), "1110");
    }

    TEST_CASE("loadInstructions() should parse NOP instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &nop = instructions[10];
        CHECK_EQ(nop.address.to_ulong(), 10);
        CHECK_EQ(nop.address.to_string(), "1010");
        CHECK_EQ(nop.opcode.to_string(), "0000");
        CHECK_EQ(nop.opcode.to_ulong(), Instructions::NOP.opcode);
        CHECK_EQ(nop.operand.to_string(), "0000");
    }

    TEST_CASE("loadInstructions() should parse ORG and DB 8-bit data instruction") {
        Assembler assembler;
        const std::vector<Assembler::Instruction> &instructions = assembler.loadInstructions("../../programs/all_instructions.asm");

        const Assembler::Instruction &db1 = instructions[11];
        CHECK_EQ(db1.address.to_ulong(), 14);
        CHECK_EQ(db1.address.to_string(), "1110");
        CHECK_EQ(db1.opcode.to_string(), "1110");
        CHECK_EQ(db1.operand.to_string(), "0110");
        CHECK_EQ(std::bitset<8>(db1.opcode.to_string() + db1.operand.to_string()).to_ulong(), 230);

        const Assembler::Instruction &db2 = instructions[12];
        CHECK_EQ(db2.address.to_ulong(), 15);
        CHECK_EQ(db2.address.to_string(), "1111");
        CHECK_EQ(db2.opcode.to_string(), "0000");
        CHECK_EQ(db2.operand.to_string(), "0101");
        CHECK_EQ(std::bitset<8>(db2.opcode.to_string() + db2.operand.to_string()).to_ulong(), 5);
    }
}
