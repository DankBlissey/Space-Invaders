#include "Hardware.h"

constexpr std::array<std::array<uint32_t, 8>, 256> buildLookupTable() {
    std::array<std::array<uint32_t, 8>, 256> table{};
    for (int byte = 0; byte < 256; byte++) {
        for (int i = 0; i < 8; i++) {
            bool bit = (byte >> i) & 1;
            table[byte][i] =  bit ? 0xFFFFFFFF : 0xFF000000;
        }
    }
    return table;
}

constexpr std::array<std::array<std::uint32_t,8>,256> lookupTable {buildLookupTable()};

// Initialize object with relevant output port functions
Hardware::Hardware() {
    // Out Port 2 specifies the shift offset
    intel8080.setOutPort(2, [&](uint8_t offset){ shiftRegister.setOffset(offset); }); 
    // Out Port 4 shift a new 8 bit value into the register
    intel8080.setOutPort(4, [&](uint8_t shiftData){ shiftRegister.shiftValueIn(shiftData); }); 
    // In Port 3 gets the shifted value from the register
    intel8080.setInPort(3, [&](){ return shiftRegister.readShiftRegister(); });
}

// Steps for one frame of execution
void Hardware::frame() {
    unsigned long long cyclesBeforeFrame {totalFrames * cyclesPerFrame};
    unsigned long long halfEndpoint {cyclesBeforeFrame + cyclesPerHalfFrame};
    unsigned long long endpoint {cyclesBeforeFrame + cyclesPerFrame};
    while (totalCycles < halfEndpoint) {
        totalCycles += intel8080.cycle();
    }
    updateFrameBuffer();
    totalFrames++;
    // insert frame rendering here as it allows for cpu init cycles to initialize vRam 
    // but it is before the CPU actually starts changing things in vRam via the interrupt
    intel8080.requestInterrupt(0xCF);
    while(totalCycles < endpoint) {
        totalCycles += intel8080.cycle();
    }
    intel8080.requestInterrupt(0xD7);
}

// Update the frame buffer with the pixel data of vRam converted to argb8888
void Hardware::updateFrameBuffer() {
    int index {0};
    for (uint8_t val : memory.getVram()) {
        const auto& expanded = lookupTable[val];
        for (std::uint32_t val : expanded) {
            frameBuffer[index] = val;
            index++;
        }
    }
}