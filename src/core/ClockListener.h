#ifndef INC_8_BIT_COMPUTER_CLOCKLISTENER_H
#define INC_8_BIT_COMPUTER_CLOCKLISTENER_H

class ClockListener {

public:
    virtual void clockTicked();
    virtual void invertedClockTicked();
};

#endif //INC_8_BIT_COMPUTER_CLOCKLISTENER_H
