#include <stdint.h>

// Function to check the sign of a byte
static bool checkSign(uint8_t value) {
	return (value & 0x80) != 0;
}


// Checks the number of 1 bits in a byte, using 5 operations
static bool checkParity(uint8_t value) {
	value ^= value >> 4;
	value &= 0x0F;
	return !((0x6996 >> value) & 1);
}

// Functions to calculate the carry and auxiliary carry flags
static bool calculateAuxCarryINR(uint8_t value) {
	return ((value & 0x0F) + 1) > 0x0F;
}

// Functions to calculate the carry and auxiliary carry flags
static bool calculateAuxCarryDCR(uint8_t value) {
	return (value & 0x0F) == 0;
}

static bool calculateAuxCarryADD(uint8_t a, uint8_t b) {
	return ((a & 0x0F) + (b & 0x0F)) > 0x0F;
}

static bool calculateCarryADD(uint8_t a, uint8_t b) {
	return (uint16_t)a + (uint16_t)b > 0xFF;
}

static bool calculateCarryADC(uint8_t a, uint8_t b, bool carryIn) {
	return (uint16_t)a + (uint16_t)b + (uint16_t)carryIn > 0xFF;
}