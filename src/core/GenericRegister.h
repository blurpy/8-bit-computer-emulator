#ifndef INC_8_BIT_COMPUTER_GENERICREGISTER_H
#define INC_8_BIT_COMPUTER_GENERICREGISTER_H

#include <memory>
#include <string>

#include "Bus.h"
#include "ClockListener.h"
#include "RegisterListener.h"
#include "ValueObserver.h"

namespace Core {

    /**
     * Generic 8-bit register with standard operations for reading from the bus and writing to the bus.
     *
     * Supports an optional listener that can be notified immediately whenever the value of the register changes.
     */
    class GenericRegister: public ClockListener {

    public:
        GenericRegister(const std::string& name, const std::shared_ptr<Bus> &bus);
        ~GenericRegister();

        /** Get the current value in the register. */
        [[nodiscard]] virtual uint8_t readValue() const;

        /** Print current value to standard out. */
        void print();

        /** Reset the register value to 0. */
        void reset();

        /** Take value from the bus on next clock tick. */
        virtual void in();

        /** Output current value to the bus. */
        virtual void out();

        /** Set a listener that will be notified when the value changes. */
        void setRegisterListener(const std::shared_ptr<RegisterListener> &newRegisterListener);

        /** Set an optional external observer of this register. */
        void setObserver(const std::shared_ptr<ValueObserver> &newObserver);

    private:
        std::string name;
        std::shared_ptr<RegisterListener> registerListener;
        std::shared_ptr<ValueObserver> observer;
        std::shared_ptr<Bus> bus;
        uint8_t value;
        bool readOnClock;

        void readFromBus();
        void writeToBus();
        void notifyObserver() const;
        void notifyListener() const;

        void clockTicked() override;
        void invertedClockTicked() override {}; // Not implemented
    };
}

#endif //INC_8_BIT_COMPUTER_GENERICREGISTER_H
