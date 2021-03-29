#ifndef INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"

class ProgramCounter: public ClockListener {

public:
    ProgramCounter(std::shared_ptr<Bus> bus);
    ~ProgramCounter();

    void print() const;
    void reset();
    void out();
    void enable();

private:
    std::shared_ptr<Bus> bus;
    uint8_t value;
    bool incrementOnClock;

    void increment();
    void readFromBus();
    void writeToBus();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
