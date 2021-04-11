#ifndef INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H
#define INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H

#include <bitset>
#include <memory>

#include "Bus.h"
#include "ClockListener.h"
#include "RegisterListener.h"

namespace Core {

    class MemoryAddressRegister: public ClockListener {

    public:
        MemoryAddressRegister(const std::shared_ptr<RegisterListener> &registerListener,
                              const std::shared_ptr<Bus> &bus);
        ~MemoryAddressRegister();

        void print() const;
        void reset();
        void program(const std::bitset<4> &address);
        void in();

    private:
        std::shared_ptr<RegisterListener> registerListener;
        std::shared_ptr<Bus> bus;
        uint8_t value;
        bool readOnClock;

        void readFromBus();

        void clockTicked() override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_MEMORYADDRESSREGISTER_H
