#pragma once
#include <cstdint>

using std::uint8_t;
using std::uint16_t;
// Shift register is used extensively for moving pixels on the screen
class ShiftRegister {
    public:
        void setOffset(uint8_t);
        void shiftValueIn(uint8_t);
        uint8_t readShiftRegister();
    private:
        uint16_t reg {0x0000};
        uint8_t offsetLatch {0x00};
};