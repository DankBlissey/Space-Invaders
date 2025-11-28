#include "CPU.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "BasicMemory.h"
using std::string;

bool loadROM(CPU& cpu, const string& fileName, size_t startAddress = 0) {
	std::ifstream romFile(fileName, std::ios::binary | std::ios::ate);
	size_t memorySize = cpu.getMemSize();
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
		cpu.writeMem(startAddress + static_cast<uint16_t>(i), buffer[i]);
	}

	return true;
}

// Handler for tests which typically run on CP/M, adds handling for exiting the program and for output of chars to the output bus
void loadTestHandler(CPU& cpu) {
	// Write newLine to output 0 and halt to exit program
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

bool runTest(CPU& cpu, const string& file) {
	std::cout << "TEST: " << file << "\n\n";
	cpu.init(0x0100, 0xFFFF);
	cpu.setOutPort(0, [&](uint8_t character){ std::cout << static_cast<char>(character); });
	if (!loadROM(cpu, file, 0x0100)) {
		return false;
	}
	unsigned long cycles {0};
	unsigned long instructions {0};
	loadTestHandler(cpu);
	while(!cpu.halted()) {
		instructions++;
		cycles += cpu.cycle();
	}
	//std::cout << "\n" << "Test finished, cycles completed: " << std::dec << cycles << "\n\n";
	return true;
}

int main()
{
	std::unique_ptr<BasicMemory> mem = std::make_unique<BasicMemory>(BasicMemory());
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>(CPU(*mem));
	runTest(*cpu, "CPU-Test-ROMs/CPUTEST.COM");
	runTest(*cpu, "CPU-Test-ROMs/TST8080.COM");
	runTest(*cpu, "CPU-Test-ROMs/8080PRE.COM");
	runTest(*cpu, "CPU-Test-ROMs/8080EXM.COM");
	return 0;
}
