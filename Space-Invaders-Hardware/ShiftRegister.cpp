#include "ShiftRegister.h"

void ShiftRegister::setOffset(uint8_t value) {
        offsetLatch = (value & 0b00000111);
}

void ShiftRegister::shiftValueIn(uint8_t value) {
    reg = (value << 8) | (reg >> 8);
}

uint8_t ShiftRegister::readShiftRegister() {
    return ((reg << offsetLatch) & 0xFF00) >> 8;
}