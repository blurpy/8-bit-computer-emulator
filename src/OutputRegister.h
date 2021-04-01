#ifndef INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H

#include <memory>

#include "Bus.h"
#include "ClockListener.h"

class OutputRegister: public ClockListener {

public:
    explicit OutputRegister(std::shared_ptr<Bus> bus);
    ~OutputRegister();

    void print() const;
    void reset();
    void in();

private:
    std::shared_ptr<Bus> bus;
    uint8_t value;
    bool readOnClock;

    void readFromBus();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
