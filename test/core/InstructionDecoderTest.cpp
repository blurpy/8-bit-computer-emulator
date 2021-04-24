#include "doctest.h"
#include "fakeit.hpp"

#include <core/Instructions.h>
#include "core/InstructionDecoder.h"

using namespace Core;

template<typename T>
std::shared_ptr<T> ptr(fakeit::Mock<T> &mock) {
    return std::shared_ptr<T>(&mock(), [](...) {});
}

/*
 * Note: fakeit is modified in order to support mocking RandomAccessMemory which use multiple inheritance.
 */

TEST_SUITE("InstructionDecoderTest") {

#ifndef _MSC_VER
    TEST_CASE("should call the correct control lines for the different instruction steps") {
        fakeit::Mock<MemoryAddressRegister> marMock;
        fakeit::Mock<ProgramCounter> pcMock;
        fakeit::Mock<RandomAccessMemory> ramMock;
        fakeit::Mock<InstructionRegister> irMock;
        fakeit::Mock<GenericRegister> aRegisterMock;
        fakeit::Mock<GenericRegister> bRegisterMock;
        fakeit::Mock<ArithmeticLogicUnit> aluMock;
        fakeit::Mock<OutputRegister> outMock;
        fakeit::Mock<FlagsRegister> flagsMock;
        fakeit::Mock<Clock> clockMock;

        InstructionDecoder instructionDecoder(ptr(marMock), ptr(pcMock), ptr(ramMock), ptr(irMock),
                                              ptr(aRegisterMock), ptr(bRegisterMock), ptr(aluMock),
                                              ptr(outMock), ptr(flagsMock), ptr(clockMock));

        fakeit::When(Method(marMock, in)).Return();
        fakeit::When(Method(pcMock, out)).Return();
        fakeit::When(Method(pcMock, enable)).Return();
        fakeit::When(Method(pcMock, jump)).Return();
        fakeit::When(Method(ramMock, in)).Return();
        fakeit::When(Method(ramMock, out)).Return();
        fakeit::When(Method(irMock, in)).Return();
        fakeit::When(Method(irMock, out)).Return();
        fakeit::When(Method(aRegisterMock, in)).Return();
        fakeit::When(Method(aRegisterMock, out)).Return();
        fakeit::When(Method(bRegisterMock, in)).Return();
        fakeit::When(Method(aluMock, out)).Return();
        fakeit::When(Method(aluMock, subtract)).Return();
        fakeit::When(Method(flagsMock, in)).Return();
        fakeit::When(Method(flagsMock, isCarryFlag)).Return(false);
        fakeit::When(Method(flagsMock, isZeroFlag)).Return(false);
        fakeit::When(Method(outMock, in)).Return();
        fakeit::When(Method(clockMock, stop)).Return();

        auto &stepCounter = dynamic_cast<StepListener &>(instructionDecoder);

        SUBCASE("FETCH pre instruction") {
            SUBCASE("FETCH step 0 should run MI|CO") {
                stepCounter.stepReady(0);

                fakeit::Verify(Method(marMock, in)).Once();
                fakeit::Verify(Method(pcMock, out)).Once();

                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("FETCH step 1 should run RO|II|CE") {
                stepCounter.stepReady(1);

                fakeit::Verify(Method(ramMock, out)).Once();
                fakeit::Verify(Method(irMock, in)).Once();
                fakeit::Verify(Method(pcMock, enable)).Once();

                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("NOP instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::NOP.opcode);

            SUBCASE("NOP step 2 should run nothing") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("NOP step 3 should run nothing") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("NOP step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("LDA instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::LDA.opcode);

            SUBCASE("LDA step 2 should run MI|IO") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(marMock, in)).Once();
                fakeit::Verify(Method(irMock, out)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("LDA step 3 should run RO|AI") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(ramMock, out)).Once();
                fakeit::Verify(Method(aRegisterMock, in)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("LDA step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("ADD instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::ADD.opcode);

            SUBCASE("ADD step 2 should run MI|IO") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(marMock, in)).Once();
                fakeit::Verify(Method(irMock, out)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("ADD step 3 should run RO|BI") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(ramMock, out)).Once();
                fakeit::Verify(Method(bRegisterMock, in)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("ADD step 4 should run AI|SO|FI") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(aRegisterMock, in)).Once();
                fakeit::Verify(Method(aluMock, out)).Once();
                fakeit::Verify(Method(flagsMock, in)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("SUB instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::SUB.opcode);

            SUBCASE("SUB step 2 should run MI|IO") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(marMock, in)).Once();
                fakeit::Verify(Method(irMock, out)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("SUB step 3 should run RO|BI") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(ramMock, out)).Once();
                fakeit::Verify(Method(bRegisterMock, in)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("SUB step 4 should run AI|S-|SO|FI") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(aRegisterMock, in)).Once();
                fakeit::Verify(Method(aluMock, subtract)).Once();
                fakeit::Verify(Method(aluMock, out)).Once();
                fakeit::Verify(Method(flagsMock, in)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("STA instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::STA.opcode);

            SUBCASE("STA step 2 should run MI|IO") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(marMock, in)).Once();
                fakeit::Verify(Method(irMock, out)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("STA step 3 should run RI|AO") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(ramMock, in)).Once();
                fakeit::Verify(Method(aRegisterMock, out)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("STA step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("LDI instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::LDI.opcode);

            SUBCASE("LDI step 2 should run IO|AI") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(irMock, out)).Once();
                fakeit::Verify(Method(aRegisterMock, in)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("LDI step 3 should run nothing") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("LDI step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("JMP instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::JMP.opcode);

            SUBCASE("JMP step 2 should run IO|CJ") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(irMock, out)).Once();
                fakeit::Verify(Method(pcMock, jump)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JMP step 3 should run nothing") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JMP step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("JC instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::JC.opcode);

            SUBCASE("JC step 2 should run nothing if carry flag not set") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(flagsMock, isCarryFlag)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JC step 2 should run IO|CJ if carry flag set") {
                fakeit::When(Method(flagsMock, isCarryFlag)).Return(true);

                stepCounter.stepReady(2);

                fakeit::Verify(Method(flagsMock, isCarryFlag)).Once();
                fakeit::Verify(Method(irMock, out)).Once();
                fakeit::Verify(Method(pcMock, jump)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JC step 3 should run nothing") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JC step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("JZ instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::JZ.opcode);

            SUBCASE("JZ step 2 should run nothing if zero flag not set") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(flagsMock, isZeroFlag)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JZ step 2 should run IO|CJ if zero flag set") {
                fakeit::When(Method(flagsMock, isZeroFlag)).Return(true);

                stepCounter.stepReady(2);

                fakeit::Verify(Method(flagsMock, isZeroFlag)).Once();
                fakeit::Verify(Method(irMock, out)).Once();
                fakeit::Verify(Method(pcMock, jump)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JZ step 3 should run nothing") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("JZ step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("OUT instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::OUT.opcode);

            SUBCASE("OUT step 2 should run AO|OI") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(aRegisterMock, out)).Once();
                fakeit::Verify(Method(outMock, in)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("OUT step 3 should run nothing") {
                stepCounter.stepReady(3);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("OUT step 4 should run nothing") {
                stepCounter.stepReady(4);

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }
        }

        SUBCASE("HLT instruction") {
            fakeit::When(Method(irMock, getOpcode)).Return(Instructions::HLT.opcode);

            SUBCASE("HLT step 2 should run HLT") {
                stepCounter.stepReady(2);

                fakeit::Verify(Method(clockMock, stop)).Once();

                fakeit::Verify(Method(irMock, getOpcode)).Once();
                fakeit::VerifyNoOtherInvocations(marMock, pcMock, ramMock, irMock, aRegisterMock, bRegisterMock,
                                                 aluMock, outMock, flagsMock, clockMock);
            }

            SUBCASE("HLT step 3 should throw exception") {
                CHECK_THROWS_WITH(stepCounter.stepReady(3),
                                  "InstructionDecoder step 3: unknown opcode 1111");
            }

            SUBCASE("HLT step 4 should throw exception") {
                CHECK_THROWS_WITH(stepCounter.stepReady(4),
                                  "InstructionDecoder step 4: unknown opcode 1111");
            }
        }

        SUBCASE("Unknown instruction step 2 should throw exception") {
            fakeit::When(Method(irMock, getOpcode)).Return(13);

            CHECK_THROWS_WITH(stepCounter.stepReady(2),
                              "InstructionDecoder step 2: unknown opcode 1101");
        }

        SUBCASE("Unknown step should throw exception") {
            CHECK_THROWS_WITH(stepCounter.stepReady(5),
                              "InstructionDecoder step is unknown: 5");
        }
    }

#else
    TEST_CASE("windows no op") {
        // This test does not work with Microsoft C/C++ compiler due to multiple inheritance issues
    }
#endif
}
