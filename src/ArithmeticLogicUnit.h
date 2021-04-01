#ifndef INC_8_BIT_COMPUTER_ARITHMETICLOGICUNIT_H
#define INC_8_BIT_COMPUTER_ARITHMETICLOGICUNIT_H

#include <memory>

#include "Register.h"
#include "RegisterListener.h"

class ArithmeticLogicUnit: public RegisterListener {

public:
    ArithmeticLogicUnit(std::shared_ptr<Register> aRegister, std::shared_ptr<Register> bRegister, std::shared_ptr<Bus> bus);
    ~ArithmeticLogicUnit();

    void print() const;
    void reset();
    void out();

private:
    std::shared_ptr<Register> aRegister;
    std::shared_ptr<Register> bRegister;
    std::shared_ptr<Bus> bus;
    uint8_t value;
    bool carry;
    bool zero;

    void writeToBus();
    void add();

    void registerValueChanged(uint8_t newValue) override;
};

#endif //INC_8_BIT_COMPUTER_ARITHMETICLOGICUNIT_H
