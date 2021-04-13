#ifndef INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
#define INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H

#include <array>
#include <bitset>
#include <memory>

#include "Bus.h"
#include "ClockListener.h"
#include "RegisterListener.h"

namespace Core {

    class RandomAccessMemory: public ClockListener, public RegisterListener {

    public:
        static const int MEMORY_SIZE = 16;

        explicit RandomAccessMemory(const std::shared_ptr<Bus> &bus);
        ~RandomAccessMemory();

        void print();
        void reset();
        void program(const std::bitset<4> &opcode, const std::bitset<4> &operand);
        virtual void in();
        virtual void out();

    private:
        std::shared_ptr<Bus> bus;
        std::array<uint8_t, MEMORY_SIZE> memory{};
        uint8_t address;
        bool readOnClock;

        void readFromBus();
        void writeToBus();

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
        void registerValueChanged(uint8_t newValue) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
