#include "SpaceInvadersMemory.h"

// Write mem doesn't allow writes to ROM and handles mirrored addresses
void SpaceInvadersMemory::write(uint16_t addr, uint8_t data) {
    if (addr > ramStart) {
        memory[ramStart + ((addr-ramStart) & ramMask)] = data;
    }
}
// Read mem handles mirrored addresses
uint8_t SpaceInvadersMemory::read(uint16_t addr) {
    if (addr > ramStart) {
        return memory[ramStart + ((addr - ramStart) & ramMask)];
    }
    return memory[addr];
}
// Write mem with ability to write to ROM: For loading ROMs
void SpaceInvadersMemory::loadMem(uint16_t addr, uint8_t data) {
    memory[addr] = data;
}
// Clear mem but don't clear the ROM
void SpaceInvadersMemory::clear() {
    std::fill(memory.begin() + ramStart, memory.end(), 0);
}