#ifndef INC_8_BIT_COMPUTER_CLOCKLISTENER_H
#define INC_8_BIT_COMPUTER_CLOCKLISTENER_H

namespace Core {

    class ClockListener {

    public:
        virtual void clockTicked() = 0;
        virtual void invertedClockTicked() = 0;
    };
}

#endif //INC_8_BIT_COMPUTER_CLOCKLISTENER_H
