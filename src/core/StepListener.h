#ifndef INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H
#define INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H

#include <cstdint>

namespace Core {

    class StepListener {

    public:
        virtual void stepReady(uint8_t step) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H
