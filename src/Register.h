#ifndef INC_8_BIT_COMPUTER_REGISTER_H
#define INC_8_BIT_COMPUTER_REGISTER_H

#include <memory>
#include <string>

#include "Bus.h"
#include "ClockListener.h"

class Register: public ClockListener {

public:
    Register(const std::string& name, std::shared_ptr<Bus> bus);
    ~Register();

    [[nodiscard]] uint8_t readValue() const;
    void print();
    void reset();
    void in();
    void out();

private:
    std::string name;
    std::shared_ptr<Bus> bus;
    uint8_t value;
    bool readOnClock;

    void readFromBus();
    void writeToBus();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_REGISTER_H
