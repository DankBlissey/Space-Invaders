#include "BasicMemory.h"

uint8_t BasicMemory::read(uint16_t addr) const {
    return mem[addr];
}

void BasicMemory::write(uint16_t addr, uint8_t data) {
    mem[addr] = data;
}

void BasicMemory::clear() {
    mem.fill(0);
}

std::size_t BasicMemory::size() const {
    return mem.size();
}