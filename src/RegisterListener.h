#ifndef INC_8_BIT_COMPUTER_EMULATOR_REGISTERLISTENER_H
#define INC_8_BIT_COMPUTER_EMULATOR_REGISTERLISTENER_H

#include <cstdint>

class RegisterListener {

public:
    virtual void registerValueChanged(uint8_t newValue);
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_REGISTERLISTENER_H
