#pragma once
#include <cstdint>
using std::uint8_t;

// Interface for implementing the sound for the hardware
class SoundChip {
    public:
        virtual ~SoundChip() = default;
        virtual void port3(uint8_t) = 0;
        virtual void port5(uint8_t) = 0;
};