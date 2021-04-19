#ifndef INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H
#define INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H

#include <cstdint>

namespace Core {

    /**
     * Interface to be implemented by those who want to be notified when the step counter is ready with a new step.
     */
    class StepListener {

    public:
        /** The specified step is now ready to be handled. */
        virtual void stepReady(uint8_t step) = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_STEPLISTENER_H
