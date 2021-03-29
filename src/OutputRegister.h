#ifndef INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H

#include <memory>

#include "Bus.h"

class OutputRegister {

public:
    OutputRegister(std::shared_ptr<Bus> bus);
    ~OutputRegister();

    void readFromBus();
    uint8_t readValue() const;
    void print();
    void reset();

private:
    std::shared_ptr<Bus> bus;
    uint8_t value;
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_OUTPUTREGISTER_H
