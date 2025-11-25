#include "CPU.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::string;

bool loadROM(CPU& cpu, const string& fileName, size_t startAddress = 0) {
	std::ifstream romFile(fileName, std::ios::binary | std::ios::ate);
	if(!romFile.is_open()) {
		std::cerr << "Error: cannot open ROM file " << fileName << "\n";
		return false;
	}
	std::streamsize size = romFile.tellg();
	romFile.seekg(0, std::ios::beg);

	if (startAddress + size > cpu.getMemSize()) {
		std::cerr << "Error: ROM too large to fit in memory\n";
		return false;
	}

	std::vector<uint8_t> buffer(size);
	if (!romFile.read(reinterpret_cast<char*>(buffer.data()), size)) {
		std::cerr << "Error: failed to read ROM data\n";
		return false;
	}

	for (size_t i = 0; i < buffer.size(); i++) {
		cpu.writeMem(startAddress + static_cast<uint16_t>(i), buffer[i]);
	}

	return true;
}


void loadTestHandler(CPU& cpu) {
	// Write 10 to output 0 and halt to exit program
	cpu.writeMem(0x0000, 0x3E); // MVI A, 0x0A : exit:
	cpu.writeMem(0x0001, 0x0A);
	cpu.writeMem(0x0002, 0xD3); // OUT 0
	cpu.writeMem(0x0003, 0x00);
	cpu.writeMem(0x0004, 0x76); // HLT
	
	// if C = 0x02 then output E reg to ouput 0, otherwise jump to putStr:
	// print value of register E
	cpu.writeMem(0x0005, 0x3E); // MVI A, 0x02 : message:
	cpu.writeMem(0x0006, 0x02);
	cpu.writeMem(0x0007, 0xB9); // CMP C
	cpu.writeMem(0x0008, 0xC2); // JNZ 0x000F
	cpu.writeMem(0x0009, 0x0F);
	cpu.writeMem(0x000A, 0x00);
	cpu.writeMem(0x000B, 0x7B); // MOV A,E :: putChr:
	cpu.writeMem(0x000C, 0xD3); // OUT 0
	cpu.writeMem(0x000D, 0x00);
	cpu.writeMem(0x000E, 0xC9); // RET

	// print value of memory pointed to by DE until a '$' character is found
	cpu.writeMem(0x000F, 0x0E); // MVI C, '$' : putStr:
	cpu.writeMem(0x0010, 0x24);
	cpu.writeMem(0x0011, 0x1A); // LDAX DE : loop:
	cpu.writeMem(0x0012, 0xB9); // CMP C
	cpu.writeMem(0x0013, 0xC2); // JNZ next
	cpu.writeMem(0x0014, 0x17);
	cpu.writeMem(0x0015, 0x00);
	cpu.writeMem(0x0016, 0xC9); // RET
	cpu.writeMem(0x0017, 0xD3); // OUT 0 : next:
	cpu.writeMem(0x0018, 0x00);
	cpu.writeMem(0x0019, 0x13); // INX DE
	cpu.writeMem(0x001A, 0xC3); // JMP loop
	cpu.writeMem(0x001B, 0x11);
	cpu.writeMem(0x001C, 0x00);
}

int main()
{
	CPU cpu = CPU(0x0100);
	cpu.init(0x0100);
	if (!loadROM(cpu, "CPU-Test-ROMs/8080EXER.COM", 0x0100)) {
		return 1;
	}
	loadTestHandler(cpu);
	while(!cpu.halted()) {
		cpu.cycle();
		std::cout << static_cast<char>(cpu.readOut(0));
	}
	return 0;
}
