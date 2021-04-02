#ifndef INC_8_BIT_COMPUTER_GENERICREGISTER_H
#define INC_8_BIT_COMPUTER_GENERICREGISTER_H

#include <memory>
#include <string>

#include "Bus.h"
#include "ClockListener.h"
#include "RegisterListener.h"

class GenericRegister: public ClockListener {

public:
    GenericRegister(const std::string& name, std::shared_ptr<Bus> bus);
    ~GenericRegister();

    [[nodiscard]] uint8_t readValue() const;
    void print();
    void reset();
    void in();
    void out();
    void setRegisterListener(std::shared_ptr<RegisterListener> newRegisterListener);

private:
    std::string name;
    std::shared_ptr<RegisterListener> registerListener;
    std::shared_ptr<Bus> bus;
    uint8_t value;
    bool readOnClock;

    void readFromBus();
    void writeToBus();

    void clockTicked() override;
};

#endif //INC_8_BIT_COMPUTER_GENERICREGISTER_H
