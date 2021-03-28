#ifndef INC_8_BIT_COMPUTER_EMULATOR_H
#define INC_8_BIT_COMPUTER_EMULATOR_H

#include <memory>

#include "ArithmeticLogicUnit.h"
#include "Bus.h"
#include "Clock.h"
#include "Register.h"

class Emulator: public ClockListener {

public:
    Emulator();
    ~Emulator();

    void run();

private:
    std::shared_ptr<Clock> clock;
    std::shared_ptr<Bus> bus;
    std::shared_ptr<Register> aRegister;
    std::shared_ptr<Register> bRegister;
    std::unique_ptr<ArithmeticLogicUnit> alu;

    void printValues();
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_H
