#ifndef INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
#define INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H

#include <array>
#include <bitset>
#include <memory>

#include "Bus.h"
#include "ClockListener.h"
#include "RegisterListener.h"

namespace Core {

    /**
     * 16 bytes of static RAM.
     *
     * Any of the 16 bytes can be read and written to, but it behaves like a regular 8-bit register in
     * that it only works with 1 byte at a time.
     *
     * To select which byte to read or write, the 4-bit memory address register must be used.
     * A 4-bit value can represent all the locations in memory from byte at address 0 to byte at address 15.
     *
     * Supports manual control like the DIP-switches using the program function.
     */
    class RandomAccessMemory: public ClockListener, public RegisterListener {

    public:
        static const int MEMORY_SIZE = 16; // 16 bytes / 16 x 8 bits

        explicit RandomAccessMemory(const std::shared_ptr<Bus> &bus);
        ~RandomAccessMemory();

        /** Print current address and all 16 values of memory to standard out. */
        void print();

        /** Reset the current address to 0. */
        void reset();

        /** Puts the specified opcode and operand into memory at the current address in manual mode. */
        void program(const std::bitset<4> &opcode, const std::bitset<4> &operand);

        /** Take the value from the bus on next clock tick and insert into the current address in memory. */
        virtual void in();

        /** Output the value at the current address in memory to the bus. */
        virtual void out();

        /** Set an optional external observer of this random access memory. */
        void setObserver(const std::shared_ptr<ValueObserver> &newObserver);

    private:
        std::shared_ptr<Bus> bus;
        std::shared_ptr<ValueObserver> observer;
        std::array<uint8_t, MEMORY_SIZE> memory{};
        uint8_t address;
        bool readOnClock;

        void readFromBus();
        void writeToBus();
        void notifyObserver() const;

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
        void registerValueChanged(uint8_t newValue) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORY_H
