#include <doctest.h>
#include <fakeit.hpp>

#include "core/Emulator.h"

using namespace Core;

template<typename T>
std::shared_ptr<T> ptr(fakeit::Mock<T> &mock) {
    return std::shared_ptr<T>(&mock(), [](...) {});
}

// Just a prettier cast from initializer_list to vector
std::vector<ControlLine> lines(const std::vector<ControlLine> &lines) {
    return lines;
}

uint8_t bits(const std::string &bits) {
    return std::bitset<8>(bits).to_ulong();
}

void runSteps(Emulator &emulator, int steps) {
    for (int i = 0; i < steps; i++) {
        emulator.singleStep();
    }
}

struct State {
    double clockHz = 0;
    bool clockOn = false;
    uint8_t busValue = 0;
    uint8_t aRegValue = 0;
    uint8_t bRegValue = 0;
    uint8_t aluValue = 0;
    bool aluCarry = false;
    bool aluZero = false;
    uint8_t marValue = 0;
    uint8_t pcValue = 0;
    uint8_t ramValue = 0;
    uint8_t irValue = 0;
    uint8_t outValue = 0;
    uint8_t stepValue = 0;
    std::vector<ControlLine> decoderLines{};
    bool flagsCarry = false;
    bool flagsZero = false;
};

TEST_SUITE("EmulatorIntegrationStepTest") {
    TEST_CASE("emulator steps should work correctly") {
        Emulator emulator;

        fakeit::Mock<ClockObserver> clockObserver;
        fakeit::Mock<ValueObserver> busObserver;
        fakeit::Mock<ValueObserver> aRegisterObserver;
        fakeit::Mock<ValueObserver> bRegisterObserver;
        fakeit::Mock<ArithmeticLogicUnitObserver> aluObserver;
        fakeit::Mock<ValueObserver> marObserver;
        fakeit::Mock<ValueObserver> pcObserver;
        fakeit::Mock<ValueObserver> ramObserver;
        fakeit::Mock<ValueObserver> irObserver;
        fakeit::Mock<ValueObserver> outObserver;
        fakeit::Mock<ValueObserver> stepObserver;
        fakeit::Mock<InstructionDecoderObserver> decoderObserver;
        fakeit::Mock<FlagsRegisterObserver> flagsObserver;

        State state;

        emulator.setClockObserver(ptr(clockObserver));
        fakeit::When(Method(clockObserver, frequencyChanged)).AlwaysDo([&](double newHz) {state.clockHz = newHz;});
        fakeit::When(Method(clockObserver, clockTicked)).AlwaysDo([&](bool newOn) {state.clockOn = newOn;});

        emulator.setBusObserver(ptr(busObserver));
        fakeit::When(Method(busObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.busValue = newValue;});

        emulator.setARegisterObserver(ptr(aRegisterObserver));
        fakeit::When(Method(aRegisterObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.aRegValue = newValue;});

        emulator.setBRegisterObserver(ptr(bRegisterObserver));
        fakeit::When(Method(bRegisterObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.bRegValue = newValue;});

        emulator.setArithmeticLogicUnitObserver(ptr(aluObserver));
        fakeit::When(Method(aluObserver, resultUpdated)).AlwaysDo([&](uint8_t newValue, bool newCarryBit, bool newZeroBit) {
            state.aluValue = newValue;
            state.aluCarry = newCarryBit;
            state.aluZero = newZeroBit;
        });

        emulator.setMemoryAddressRegisterObserver(ptr(marObserver));
        fakeit::When(Method(marObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.marValue = newValue;});

        emulator.setProgramCounterObserver(ptr(pcObserver));
        fakeit::When(Method(pcObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.pcValue = newValue;});

        emulator.setRandomAccessMemoryObserver(ptr(ramObserver));
        fakeit::When(Method(ramObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.ramValue = newValue;});

        emulator.setInstructionRegisterObserver(ptr(irObserver));
        fakeit::When(Method(irObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.irValue = newValue;});

        emulator.setOutputRegisterObserver(ptr(outObserver));
        fakeit::When(Method(outObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.outValue = newValue;});

        emulator.setStepCounterObserver(ptr(stepObserver));
        fakeit::When(Method(stepObserver, valueUpdated)).AlwaysDo([&](uint8_t newValue) {state.stepValue = newValue;});

        emulator.setInstructionDecoderObserver(ptr(decoderObserver));
        fakeit::When(Method(decoderObserver, controlWordUpdated)).AlwaysDo([&](const std::vector<ControlLine> &newLines) {
            state.decoderLines = newLines;
        });

        emulator.setFlagsRegisterObserver(ptr(flagsObserver));
        fakeit::When(Method(flagsObserver, flagsUpdated)).AlwaysDo([&](bool newCarryFlag, bool newZeroFlag) {
            state.flagsCarry = newCarryFlag;
            state.flagsZero = newZeroFlag;
        });

        emulator.load("../../programs/subtract_two_numbers.asm");
        emulator.setFrequency(100);

        SUBCASE("LDA 14 step 0") {
            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0000"), state.pcValue);
            CHECK_EQ(bits("00000000"), state.busValue);
            CHECK_EQ(bits("00000000"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00000000"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(true, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0000"), state.marValue);
            CHECK_EQ(bits("00011110"), state.ramValue);
            CHECK_EQ(bits("000"), state.stepValue);
            CHECK_EQ(bits("00000000"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::MI, ControlLine::CO}), state.decoderLines);
        }

        SUBCASE("LDA 14 step 1") {
            runSteps(emulator, 1);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0000"), state.pcValue);
            CHECK_EQ(bits("00011110"), state.busValue);
            CHECK_EQ(bits("00000000"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00000000"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(true, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0000"), state.marValue);
            CHECK_EQ(bits("00011110"), state.ramValue);
            CHECK_EQ(bits("001"), state.stepValue);
            CHECK_EQ(bits("00000000"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::RO, ControlLine::II, ControlLine::CE}), state.decoderLines);
        }

        SUBCASE("LDA 14 step 2") {
            runSteps(emulator, 2);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0001"), state.pcValue);
            CHECK_EQ(bits("00001110"), state.busValue);
            CHECK_EQ(bits("00000000"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00000000"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(true, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0000"), state.marValue);
            CHECK_EQ(bits("00011110"), state.ramValue);
            CHECK_EQ(bits("010"), state.stepValue);
            CHECK_EQ(bits("00011110"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::MI, ControlLine::IO}), state.decoderLines);
        }

        SUBCASE("LDA 14 step 3") {
            runSteps(emulator, 3);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0001"), state.pcValue);
            CHECK_EQ(bits("00011110"), state.busValue);
            CHECK_EQ(bits("00000000"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00000000"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(true, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("1110"), state.marValue);
            CHECK_EQ(bits("00011110"), state.ramValue);
            CHECK_EQ(bits("011"), state.stepValue);
            CHECK_EQ(bits("00011110"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::RO, ControlLine::AI}), state.decoderLines);
        }

        SUBCASE("LDA 14 step 4") {
            runSteps(emulator, 4);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0001"), state.pcValue);
            CHECK_EQ(bits("00000000"), state.busValue);
            CHECK_EQ(bits("00011110"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("1110"), state.marValue);
            CHECK_EQ(bits("00011110"), state.ramValue);
            CHECK_EQ(bits("100"), state.stepValue);
            CHECK_EQ(bits("00011110"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({}), state.decoderLines);
        }

        SUBCASE("SUB 15 step 0") {
            runSteps(emulator, 5);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0001"), state.pcValue);
            CHECK_EQ(bits("00000001"), state.busValue);
            CHECK_EQ(bits("00011110"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("1110"), state.marValue);
            CHECK_EQ(bits("00011110"), state.ramValue);
            CHECK_EQ(bits("000"), state.stepValue);
            CHECK_EQ(bits("00011110"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::MI, ControlLine::CO}), state.decoderLines);
        }

        SUBCASE("SUB 15 step 1") {
            runSteps(emulator, 6);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0001"), state.pcValue);
            CHECK_EQ(bits("00111111"), state.busValue);
            CHECK_EQ(bits("00011110"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0001"), state.marValue);
            CHECK_EQ(bits("00111111"), state.ramValue);
            CHECK_EQ(bits("001"), state.stepValue);
            CHECK_EQ(bits("00011110"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::RO, ControlLine::II, ControlLine::CE}), state.decoderLines);
        }

        SUBCASE("SUB 15 step 2") {
            runSteps(emulator, 7);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0010"), state.pcValue);
            CHECK_EQ(bits("00001111"), state.busValue);
            CHECK_EQ(bits("00011110"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0001"), state.marValue);
            CHECK_EQ(bits("00111111"), state.ramValue);
            CHECK_EQ(bits("010"), state.stepValue);
            CHECK_EQ(bits("00111111"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::MI, ControlLine::IO}), state.decoderLines);
        }

        SUBCASE("SUB 15 step 3") {
            runSteps(emulator, 8);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0010"), state.pcValue);
            CHECK_EQ(bits("00001100"), state.busValue);
            CHECK_EQ(bits("00011110"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("1111"), state.marValue);
            CHECK_EQ(bits("00001100"), state.ramValue);
            CHECK_EQ(bits("011"), state.stepValue);
            CHECK_EQ(bits("00111111"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::RO, ControlLine::BI}), state.decoderLines);
        }

        SUBCASE("SUB 15 step 4") {
            runSteps(emulator, 9);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0010"), state.pcValue);
            CHECK_EQ(bits("00010010"), state.busValue);
            CHECK_EQ(bits("00011110"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00010010"), state.aluValue);
            CHECK_EQ(true, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("1111"), state.marValue);
            CHECK_EQ(bits("00001100"), state.ramValue);
            CHECK_EQ(bits("100"), state.stepValue);
            CHECK_EQ(bits("00111111"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::AI, ControlLine::SM,ControlLine::SO, ControlLine::FI}), state.decoderLines);
        }

        SUBCASE("OUT step 0") {
            runSteps(emulator, 10);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0010"), state.pcValue);
            CHECK_EQ(bits("00000010"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("1111"), state.marValue);
            CHECK_EQ(bits("00001100"), state.ramValue);
            CHECK_EQ(bits("000"), state.stepValue);
            CHECK_EQ(bits("00111111"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::MI, ControlLine::CO}), state.decoderLines);
        }

        SUBCASE("OUT step 1") {
            runSteps(emulator, 11);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0010"), state.pcValue);
            CHECK_EQ(bits("11100000"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0010"), state.marValue);
            CHECK_EQ(bits("11100000"), state.ramValue);
            CHECK_EQ(bits("001"), state.stepValue);
            CHECK_EQ(bits("00111111"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::RO, ControlLine::II, ControlLine::CE}), state.decoderLines);
        }

        SUBCASE("OUT step 2") {
            runSteps(emulator, 12);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0011"), state.pcValue);
            CHECK_EQ(bits("00010010"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0010"), state.marValue);
            CHECK_EQ(bits("11100000"), state.ramValue);
            CHECK_EQ(bits("010"), state.stepValue);
            CHECK_EQ(bits("11100000"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::AO, ControlLine::OI}), state.decoderLines);
        }

        SUBCASE("OUT step 3") {
            runSteps(emulator, 13);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0011"), state.pcValue);
            CHECK_EQ(bits("00000000"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0010"), state.marValue);
            CHECK_EQ(bits("11100000"), state.ramValue);
            CHECK_EQ(bits("011"), state.stepValue);
            CHECK_EQ(bits("11100000"), state.irValue);
            CHECK_EQ(18, state.outValue);
            CHECK_EQ(lines({}), state.decoderLines);
        }

        SUBCASE("OUT step 4") {
            runSteps(emulator, 14);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0011"), state.pcValue);
            CHECK_EQ(bits("00000000"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0010"), state.marValue);
            CHECK_EQ(bits("11100000"), state.ramValue);
            CHECK_EQ(bits("100"), state.stepValue);
            CHECK_EQ(bits("11100000"), state.irValue);
            CHECK_EQ(18, state.outValue);
            CHECK_EQ(lines({}), state.decoderLines);
        }

        SUBCASE("HLT step 0") {
            runSteps(emulator, 15);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0011"), state.pcValue);
            CHECK_EQ(bits("00000011"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0010"), state.marValue);
            CHECK_EQ(bits("11100000"), state.ramValue);
            CHECK_EQ(bits("000"), state.stepValue);
            CHECK_EQ(bits("11100000"), state.irValue);
            CHECK_EQ(18, state.outValue);
            CHECK_EQ(lines({ControlLine::MI, ControlLine::CO}), state.decoderLines);
        }

        SUBCASE("HLT step 1") {
            runSteps(emulator, 16);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0011"), state.pcValue);
            CHECK_EQ(bits("11110000"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0011"), state.marValue);
            CHECK_EQ(bits("11110000"), state.ramValue);
            CHECK_EQ(bits("001"), state.stepValue);
            CHECK_EQ(bits("11100000"), state.irValue);
            CHECK_EQ(18, state.outValue);
            CHECK_EQ(lines({ControlLine::RO, ControlLine::II, ControlLine::CE}), state.decoderLines);
        }

        SUBCASE("HLT step 2") {
            runSteps(emulator, 17);

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0100"), state.pcValue);
            CHECK_EQ(bits("00000000"), state.busValue);
            CHECK_EQ(bits("00010010"), state.aRegValue);
            CHECK_EQ(bits("00001100"), state.bRegValue);
            CHECK_EQ(bits("00011110"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(false, state.aluZero);
            CHECK_EQ(true, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0011"), state.marValue);
            CHECK_EQ(bits("11110000"), state.ramValue);
            CHECK_EQ(bits("010"), state.stepValue);
            CHECK_EQ(bits("11110000"), state.irValue);
            CHECK_EQ(18, state.outValue);
            CHECK_EQ(lines({ControlLine::HLT}), state.decoderLines);
        }

        SUBCASE("HLT has no more steps") {
            runSteps(emulator, 18);

            // Same as previous
            CHECK_EQ(bits("010"), state.stepValue);
        }

        SUBCASE("reload should set state back to LDA 14 step 0") {
            runSteps(emulator, 20);

            // Same as previous
            CHECK_EQ(bits("010"), state.stepValue);

            emulator.reload();

            CHECK_EQ(100, state.clockHz);
            CHECK_EQ(false, state.clockOn);
            CHECK_EQ(bits("0000"), state.pcValue);
            CHECK_EQ(bits("00000000"), state.busValue);
            CHECK_EQ(bits("00000000"), state.aRegValue);
            CHECK_EQ(bits("00000000"), state.bRegValue);
            CHECK_EQ(bits("00000000"), state.aluValue);
            CHECK_EQ(false, state.aluCarry);
            CHECK_EQ(true, state.aluZero);
            CHECK_EQ(false, state.flagsCarry);
            CHECK_EQ(false, state.flagsZero);
            CHECK_EQ(bits("0000"), state.marValue);
            CHECK_EQ(bits("00011110"), state.ramValue);
            CHECK_EQ(bits("000"), state.stepValue);
            CHECK_EQ(bits("00000000"), state.irValue);
            CHECK_EQ(0, state.outValue);
            CHECK_EQ(lines({ControlLine::MI, ControlLine::CO}), state.decoderLines);
        }
    }
}
