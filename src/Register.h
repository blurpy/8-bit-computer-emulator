#ifndef INC_8_BIT_COMPUTER_REGISTER_H
#define INC_8_BIT_COMPUTER_REGISTER_H

#include <memory>
#include <string>

#include "Bus.h"

class Register {

public:
    Register(const std::string& name, std::shared_ptr<Bus> bus);
    ~Register();

    void readFromBus();
    void writeToBus();
    uint8_t readValue() const;

    void print();

private:
    std::string name;
    std::shared_ptr<Bus> bus;
    uint8_t value;
};

#endif //INC_8_BIT_COMPUTER_REGISTER_H
