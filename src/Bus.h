#ifndef INC_8_BIT_COMPUTER_BUS_H
#define INC_8_BIT_COMPUTER_BUS_H

class Bus {

public:
    Bus();
    ~Bus();

    [[nodiscard]] uint8_t read() const;
    void write(uint8_t newValue);
    void print() const;
    void reset();

private:
    uint8_t value;
};

#endif //INC_8_BIT_COMPUTER_BUS_H
