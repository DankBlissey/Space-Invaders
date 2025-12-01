#include "SpaceInvadersMemory.h"

SpaceInvadersMemory::SpaceInvadersMemory() {
    buildPointerTable();
}
// Build table of pointers to relevant memory addresses. Enables very fast read and write while keeping memory arrays separate
void SpaceInvadersMemory::buildPointerTable() {
    for (uint16_t addr = 0; addr < memorySize; addr++) {
        if (addr < ramStart) {
            memoryPointers[addr] = &rom[addr];
        } else if (addr < vRamStart) {
            memoryPointers[addr] = &ram[addr - ramStart];
        } else {
            memoryPointers[addr] = &vRam[addr- vRamStart];
        }
    }
}
// Read byte from memory
uint8_t SpaceInvadersMemory::read(uint16_t addr) const {
    return *memoryPointers[addr & ramMask];
}
// Write a byte to memory
void SpaceInvadersMemory::write(uint16_t addr, uint8_t data) {
    uint16_t masked = addr & ramMask;
    if (masked < ramStart) {
        // ROM is read-only
        return;
    }
    *memoryPointers[masked] = data;
}
// Clear ram and Vram
void SpaceInvadersMemory::clear() {
    ram.fill(0);
    vRam.fill(0);
}
// return the size of memory
std::size_t SpaceInvadersMemory::size() const {
    return static_cast<std::size_t>(memorySize);
}

void SpaceInvadersMemory::writeRom(uint16_t addr, uint8_t data) {
    if (addr < ramStart) {
        rom[addr] = data;
    }
}

const std::array<uint8_t, SpaceInvadersMemory::vRamSize>& SpaceInvadersMemory::getVram() const {
    return vRam;
}