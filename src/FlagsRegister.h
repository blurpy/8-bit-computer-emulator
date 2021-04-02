#ifndef INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTER_H

#include "ArithmeticLogicUnit.h"
#include "ClockListener.h"

class FlagsRegister: public ClockListener {

public:
    FlagsRegister(std::shared_ptr<ArithmeticLogicUnit> alu);
    ~FlagsRegister();

    void print() const;
    void reset();
    void in();
    [[nodiscard]] bool isCarryFlag() const;
    [[nodiscard]] bool isZeroFlag() const;

private:
    std::shared_ptr<ArithmeticLogicUnit> alu;
    bool readOnClock;
    bool carryFlag;
    bool zeroFlag;

    void readFromAlu();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_FLAGSREGISTER_H
