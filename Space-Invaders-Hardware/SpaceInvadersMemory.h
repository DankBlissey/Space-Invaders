#pragma once
#include "../Intel8080/Memory.h"
#include <array>

// Variant of memory that handles read-only access and RAM mirroring
class SpaceInvadersMemory : public Memory {
    public:
        SpaceInvadersMemory();
        uint8_t read(uint16_t) const override;
        void write(uint16_t, uint8_t) override;
        void clear() override;
        std::size_t size() const override;

        void writeRom(uint16_t, uint8_t);
    private:
        static constexpr uint16_t romSize {0x2000};
        static constexpr uint16_t ramSize {0x0400};
        static constexpr uint16_t vRamSize {0x1C00};
        static constexpr uint16_t ramStart {romSize};
        static constexpr uint16_t vRamStart {static_cast<uint16_t>(ramStart + ramSize)};
        static constexpr uint16_t totalRamSize {static_cast<uint16_t>(ramSize + vRamSize)};
        static constexpr uint16_t memorySize {static_cast<uint16_t>(totalRamSize + romSize)};
        static constexpr uint16_t ramMask {static_cast<uint16_t>(memorySize - 1)};

        std::array<uint8_t, romSize> rom {0};
        std::array<uint8_t, ramSize> ram {0};
        std::array<uint8_t, vRamSize> vRam {0};
        
        std::array<uint8_t*, memorySize> memoryPointers {};

        void buildPointerTable();
        
};