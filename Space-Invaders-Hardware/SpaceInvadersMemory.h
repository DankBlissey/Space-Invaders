#include "../Intel8080/Memory.h"

// Intel 8080 CPU with setup for barring writes to ROM and for handling RAM Mirroring
// Also features a shift register
class SpaceInvadersMemory : Memory {
    public: 
        uint8_t read(uint16_t);
        void write(uint16_t, uint8_t);
        void clear();
        void loadMem(uint16_t, uint8_t);
    private:
        const uint16_t romStart {0x0000};
        const uint16_t romEnd {0x1FFF};
        const uint16_t ramStart {0x2000};
        const uint16_t ramEnd {0x23FF};
        const uint16_t vRamStart {0x2400};
        const uint16_t vRamEnd {0x3FFF};
        const uint16_t ramSize {static_cast<uint16_t>(vRamEnd - ramStart + 1)};
        const uint16_t ramMask {static_cast<uint16_t>(ramSize - 1)};
};