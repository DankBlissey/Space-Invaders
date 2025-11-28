#include "Hardware.h"

// Initialize object with relevant output port functions
Hardware::Hardware() {
    // Out Port 2 specifies the shift offset
    intel8080.setOutPort(2, [&](uint8_t offset){ shiftRegister.setOffset(offset); }); 
    // Out Port 4 shift a new 8 bit value into the register
    intel8080.setOutPort(4, [&](uint8_t shiftData){ shiftRegister.shiftValueIn(shiftData); }); 
    // In Port 3 gets the shifted value from the register
    intel8080.setInPort(3, [&](){ return shiftRegister.readShiftRegister(); });
}