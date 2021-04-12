#include "doctest.h"
#include "fakeit.hpp"

#include <core/Instructions.h>
#include "core/InstructionDecoder.h"

using namespace Core;

template<typename T>
std::shared_ptr<T> ptr(fakeit::Mock<T> &mock) {
    return std::shared_ptr<T>(&mock(), [](...) {});
}

TEST_SUITE("InstructionDecoderTest") {
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
        fakeit::When(Method(ramMock, out)).Return();
        fakeit::When(Method(irMock, in)).Return();
        fakeit::When(Method(irMock, out)).Return();
        fakeit::When(Method(aRegisterMock, in)).Return();
        fakeit::When(Method(bRegisterMock, in)).Return();
        fakeit::When(Method(aluMock, out)).Return();
        fakeit::When(Method(flagsMock, in)).Return();

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
    }
}
