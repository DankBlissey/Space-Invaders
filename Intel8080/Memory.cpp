#include "Memory.h"

void Memory::write(uint16_t addr, uint8_t data) {
    memory[addr] = data;
}

uint8_t Memory::read(uint16_t addr) {
    return memory[addr];
}

void Memory::clear() {
    memory.fill(0);
}

std::size_t Memory::size() {
    return memory.size();
}