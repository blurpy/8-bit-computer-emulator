#ifndef INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"

class ProgramCounter: public ClockListener {

public:
    explicit ProgramCounter(std::shared_ptr<Bus> bus);
    ~ProgramCounter();

    void print() const;
    void reset();
    void out();
    void enable();
    void jump();

private:
    std::shared_ptr<Bus> bus;
    uint8_t value;
    bool incrementOnClock;
    bool readOnClock;

    void increment();
    void readFromBus();
    void writeToBus();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_PROGRAMCOUNTER_H
