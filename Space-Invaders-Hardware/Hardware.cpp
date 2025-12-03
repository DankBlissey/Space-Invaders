#include "Hardware.h"
#include <fstream>
#include <iostream>

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

void Hardware::setUpPorts() {
    // Out Port 2 specifies the shift offset
    intel8080.setOutPort(2, [&](uint8_t offset){ shiftRegister.setOffset(offset); });
    // Out Port 3 specifies certain discrete sounds, not added yet
    intel8080.setOutPort(3, [&](uint8_t soundData) {});
    // Out Port 4 shift a new 8 bit value into the register
    intel8080.setOutPort(4, [&](uint8_t shiftData){ shiftRegister.shiftValueIn(shiftData); });
    // Out Port 5 specifis other discrete sounds, not added yet
    intel8080.setOutPort(5, [&](uint8_t soundData){});
    // Out Port 6 resets the cpu, not added yet
    intel8080.setOutPort(6, [&](uint8_t data){});

    // In Ports 0, 1, and 2 specify inputs and settings for the cabinet
    intel8080.setInPort(0, [&](){ return inputPort0(); });
    intel8080.setInPort(1, [&](){ return inputPort1(); });
    intel8080.setInPort(2, [&](){ return inputPort2(); });
    // In Port 3 gets the shifted value from the register
    intel8080.setInPort(3, [&](){ return shiftRegister.readShiftRegister(); });
}

bool Hardware::loadROMFile(const std::string& fileName, size_t startAddress) {
    std::ifstream romFile(fileName, std::ios::binary | std::ios::ate);
	size_t memorySize = memory->size();
	if(!romFile.is_open()) {
		std::cerr << "Error: cannot open ROM file " << fileName << "\n";
		return false;
	}
	std::streamsize size = romFile.tellg();
	romFile.seekg(0, std::ios::beg);

	if (startAddress + size > memorySize) {
		std::cerr << "Error: ROM too large to fit in memory\n";
		return false;
	}

	std::vector<uint8_t> buffer(size);
	if (!romFile.read(reinterpret_cast<char*>(buffer.data()), size)) {
		std::cerr << "Error: failed to read ROM data\n";
		return false;
	}

	for (size_t i = 0; i < buffer.size(); i++) {
		memory->writeRom(startAddress + static_cast<uint16_t>(i), buffer[i]);
	}

	return true;
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
    for (uint8_t val : memory->getVram()) {
        const auto& expanded = lookupTable[val];
        for (std::uint32_t val : expanded) {
            frameBuffer[index] = val;
            index++;
        }
    }
}

bool Hardware::getCredit() {
    bool value = credit;
    credit = false;
    return value;
}

uint8_t Hardware::inputPort0() {
    return (right << 6) |
        (left << 5) |
        (fire << 4) |
        (selfTest << 0) |
        (0b00001110);
}

uint8_t Hardware::inputPort1() {
    uint8_t outputByte = (0 << 7) |
        (playerOneRight << 6) |
        (playerOneLeft << 5) |
        (playerOneShot << 4) |
        (1 << 3) |
        (onePlayerStart << 2) |
        (twoPlayerStart << 1) |
        (getCredit() << 0);
    return outputByte;
}

uint8_t Hardware::inputPort2() {
    return (disableCoinInfo << 7) |
        (playerTwoRight << 6) |
        (playerTwoLeft << 5) |
        (playerTwoShot << 4) |
        (extraShipAtHighOrLowScore << 3) |
        (tilt << 2) |
        (shipsBitM << 1) |
        (shipsBitL << 0);
}