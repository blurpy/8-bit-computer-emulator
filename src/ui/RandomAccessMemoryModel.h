#ifndef INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORYMODEL_H
#define INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORYMODEL_H

#include <array>
#include <memory>
#include <string>

#include "ValueModel.h"

namespace UI {

    class RandomAccessMemoryModel: public Core::ValueObserver {

    public:
        static const int MEMORY_SIZE = 16;

        explicit RandomAccessMemoryModel(const std::shared_ptr<ValueModel> &memoryAddressRegister);
        ~RandomAccessMemoryModel();

        [[nodiscard]] std::string getRenderText() const;
        [[nodiscard]] std::array<std::string, MEMORY_SIZE> getRenderTextFull() const;

    private:
        std::array<uint8_t, MEMORY_SIZE> memory{};
        std::shared_ptr<ValueModel> memoryAddressRegister;
        uint8_t value;

        void valueUpdated(uint8_t newValue) override;
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_RANDOMACCESSMEMORYMODEL_H
