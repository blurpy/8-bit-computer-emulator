#ifndef INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H
#define INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H

#include <cstdint>

class StepListener {

public:
    virtual void stepReady(uint8_t step);
};

#endif //INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H
