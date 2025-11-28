#include "CPU.h"
#include "OpcodeTableValues.cpp"

// Array of function pointers for each opcode
const CPU::OpFunc CPU::functptr[256] = {
	&CPU::nop, & CPU::lxiB, & CPU::staxB, & CPU::inxB, & CPU::inrB, & CPU::dcrB, & CPU::mviB, & CPU::rlc, & CPU::nop, & CPU::dadB, & CPU::ldaxB, & CPU::dcxB, & CPU::inrC, & CPU::dcrC, & CPU::mviC, & CPU::rrc,
		& CPU::nop, & CPU::lxiD, & CPU::staxD, & CPU::inxD, & CPU::inrD, & CPU::dcrD, & CPU::mviD, & CPU::ral, & CPU::nop, & CPU::dadD, & CPU::ldaxD, & CPU::dcxD, & CPU::inrE, & CPU::dcrE, & CPU::mviE, & CPU::rar,
		& CPU::nop, & CPU::lxiH, & CPU::shld, & CPU::inxH, & CPU::inrH, & CPU::dcrH, & CPU::mviH, & CPU::daa, & CPU::nop, & CPU::dadH, & CPU::lhld, & CPU::dcxH, & CPU::inrL, & CPU::dcrL, & CPU::mviL, & CPU::cma,
		& CPU::nop, & CPU::lxiSP, & CPU::sta, & CPU::inxSP, & CPU::inrM, & CPU::dcrM, & CPU::mviM, & CPU::stc, & CPU::nop, & CPU::dadSP, & CPU::lda, & CPU::dcxSP, & CPU::inrA, & CPU::dcrA, & CPU::mviA, & CPU::cmc,
		& CPU::movBB, & CPU::movBC, & CPU::movBD, & CPU::movBE, & CPU::movBH, & CPU::movBL, & CPU::movBM, & CPU::movBA, & CPU::movCB, & CPU::movCC, & CPU::movCD, & CPU::movCE, & CPU::movCH, & CPU::movCL, & CPU::movCM, & CPU::movCA,
		& CPU::movDB, & CPU::movDC, & CPU::movDD, & CPU::movDE, & CPU::movDH, & CPU::movDL, & CPU::movDM, & CPU::movDA, & CPU::movEB, & CPU::movEC, & CPU::movED, & CPU::movEE, & CPU::movEH, & CPU::movEL, & CPU::movEM, & CPU::movEA,
		& CPU::movHB, & CPU::movHC, & CPU::movHD, & CPU::movHE, & CPU::movHH, & CPU::movHL, & CPU::movHM, & CPU::movHA, & CPU::movLB, & CPU::movLC, & CPU::movLD, & CPU::movLE, & CPU::movLH, & CPU::movLL, & CPU::movLM, & CPU::movLA,
		& CPU::movMB, & CPU::movMC, & CPU::movMD, & CPU::movME, & CPU::movMH, & CPU::movML, & CPU::hlt, & CPU::movMA, & CPU::movAB, & CPU::movAC, & CPU::movAD, & CPU::movAE, & CPU::movAH, & CPU::movAL, & CPU::movAM, & CPU::movAA,
		& CPU::addB, & CPU::addC, & CPU::addD, & CPU::addE, & CPU::addH, & CPU::addL, & CPU::addM, & CPU::addA, & CPU::adcB, & CPU::adcC, & CPU::adcD, & CPU::adcE, & CPU::adcH, & CPU::adcL, & CPU::adcM, & CPU::adcA,
		& CPU::subB, & CPU::subC, & CPU::subD, & CPU::subE, & CPU::subH, & CPU::subL, & CPU::subM, & CPU::subA, & CPU::sbbB, & CPU::sbbC, & CPU::sbbD, & CPU::sbbE, & CPU::sbbH, & CPU::sbbL, & CPU::sbbM, & CPU::sbbA,
		& CPU::anaB, & CPU::anaC, & CPU::anaD, & CPU::anaE, & CPU::anaH, & CPU::anaL, & CPU::anaM, & CPU::anaA, & CPU::xraB, & CPU::xraC, & CPU::xraD, & CPU::xraE, & CPU::xraH, & CPU::xraL, & CPU::xraM, & CPU::xraA,
		& CPU::oraB, & CPU::oraC, & CPU::oraD, & CPU::oraE, & CPU::oraH, & CPU::oraL, & CPU::oraM, & CPU::oraA, & CPU::cmpB, & CPU::cmpC, & CPU::cmpD, & CPU::cmpE, & CPU::cmpH, & CPU::cmpL, & CPU::cmpM, & CPU::cmpA,
		& CPU::rnz, & CPU::popB, & CPU::jnz, & CPU::jmp, & CPU::cnz, & CPU::pushB, & CPU::adi, & CPU::rst0, & CPU::rz, & CPU::ret, & CPU::jz, & CPU::jmp, & CPU::cz, & CPU::call, & CPU::aci, & CPU::rst1,
		& CPU::rnc, & CPU::popD, & CPU::jnc, & CPU::outFunc, & CPU::cnc, & CPU::pushD, & CPU::sui, & CPU::rst2, & CPU::rc, & CPU::ret, & CPU::jc, & CPU::inFunc, & CPU::cc, & CPU::call, & CPU::sbi, & CPU::rst3,
		& CPU::rpo, & CPU::popH, & CPU::jpo, & CPU::xthl, & CPU::cpo, & CPU::pushH, & CPU::ani, & CPU::rst4, & CPU::rpe, & CPU::pchl, & CPU::jpe, & CPU::xchg, & CPU::cpe, & CPU::call, & CPU::xri, & CPU::rst5,
		& CPU::rp, & CPU::popPSW, & CPU::jp, & CPU::di, & CPU::cp, & CPU::pushPSW, & CPU::ori, & CPU::rst6, & CPU::rm, & CPU::sphl, & CPU::jm, & CPU::ei, & CPU::cm, & CPU::call, & CPU::cpi, & CPU::rst7
};
// Initialize the CPU with memory
CPU::CPU(Memory& memory) : mem(memory){}

// Initialise the CPU with Zero values
void CPU::init() {
	pc = 0;
	sp = 0;
	// Clear memory
	clearMem();
	extraCycles = 0;
	B = C = D = E = H = L = A = 0;
	interruptPending = STOPPED = INTE = Sign = Zero = AuxCarry = Parity = Carry = false;
	interruptVector = 0;
}
// Initialize the CPU with specified program counter and stack pointer
void CPU::init(uint16_t programCounter, uint16_t stackPointer) {
	init();
	pc = programCounter;
	sp = stackPointer;
}

void CPU::reset() {
	pc = 0;
	INTE = STOPPED = interruptPending = false;
}

void CPU::requestInterrupt(uint8_t vector) {
	if (INTE) {
		interruptPending = true;
		interruptVector = vector;
		STOPPED = false;
	}
}

void CPU::stopInterrupt() {
	interruptPending = false;
}

// Execute a single CPU cycle
uint8_t CPU::cycle() {
	extraCycles = 0;
	if (interruptPending) {
		// Use the interrupt opcode provided
		currentInstruction = readMem(interruptVector);
		// Execute the opcode
		(this->*functptr[currentInstruction])();
		// Reset the INTE flip flop
		interruptPending = false;
		INTE = false;
		interruptVector = 0x00;
		// pc is not incremented this time (idea is that it will be pushed onto the stack with rst)
		return opcodeCycles[currentInstruction] + extraCycles;
	}
	if (STOPPED) {
		// CPU is halted, return 4 as clock still continues despite the CPU not doing anything
		return 4;
	}
	// Fetch the opcode
	currentInstruction = readMem(pc);
	// Increment the program counter
	pc += opcodeByteLength[currentInstruction];
	// Execute the opcode
	(this->*functptr[currentInstruction])();
	// return number of cycles that instruction took
	return opcodeCycles[currentInstruction] + extraCycles;
}

bool CPU::halted() {
	return STOPPED;
}

void CPU::setInPort(uint8_t port, InHandler handler) {
	in[port] = handler;
}

void CPU::setOutPort(uint8_t port, OutHandler handler) {
	out[port] = handler;
}
// Call the function at port x on in to simulate reading via the input bus
uint8_t CPU::readIn(uint8_t port) {
	if (in[port]) {
		return in[port]();
	}
	return 0xFF;
}
// Call the function at port x on out to simulate outputting via the output bus
void CPU::writeOut(uint8_t port, uint8_t value) {
	if (out[port]) {
		out[port](value);
	}
}
// Read a byte of memory
uint8_t CPU::readMem(uint16_t addr) const {
	return mem.read(addr);
}
// Write a byte of memory
void CPU::writeMem(uint16_t addr, uint8_t data) {
	mem.write(addr, data);
}
// Clear memory
void CPU::clearMem() {
	mem.clear();
}
// get size of memory
size_t CPU::getMemSize() {
	return mem.size();
}
// Write a 16-bit value to the BC register pair
void CPU::writePairB(uint16_t data) {
	B = (data & 0xFF00) >> 8;
	C = data & 0x00FF;
}
// Read a 16-bit value from the BC register pair
uint16_t CPU::readPairB() {
	return (B << 8) | C;
}
// Write a 16-bit value to the DE register pair
void CPU::writePairD(uint16_t data) {
	D = (data & 0xFF00) >> 8;
	E = data & 0x00FF;
}
// Read a 16-bit value from the DE register pair
uint16_t CPU::readPairD() {
	return (D << 8) | E;
}
// Write a 16-bit value to the HL register pair
void CPU::writePairH(uint16_t data) {
	H = (data & 0xFF00) >> 8;
	L = data & 0x00FF;
}
// Read a 16-bit value from the HL register pair
uint16_t CPU::readPairH() {
	return (H << 8) | L;
}
// Write a 16 bit value to the AF register pair (Accumulator and then the flags)
void CPU::writePairPSW(uint16_t data) {
	A = (data & 0xFF00) >> 8;
	uint8_t Flags = data & 0x00FF;
	// Sign, Zero, 0, AuxCarry, 0, Parity, 1, Carry
	Sign = Flags & 0b10000000;
	Zero = Flags & 0b01000000;
	// 0b00100000 is always 0
	AuxCarry = Flags & 0b00010000;
	// 0b00001000 is always 0
	Parity = Flags & 0b00000100;
	// 0b00000010 is always 1
	Carry = Flags & 0b00000001;
}
// Read a 16 bit value from the AF register pair
uint16_t CPU::readPairPSW() {
	return (A << 8) |
		(Sign << 7) |
		(Zero << 6) |
		(0 << 5) |
		(AuxCarry << 4) |
		(0 << 3) |
		(Parity << 2) |
		(1 << 1) |
		(Carry << 0);
}
// Push a 16-bit value onto the stack
void CPU::stackPush(uint16_t data) {
	writeMem(sp-1, (data & 0xFF00) >> 8);
	writeMem(sp-2, data & 0x00FF);
	sp -= 2;
}
// Pop a 16-bit value from the stack
uint16_t CPU::stackPop() {
	uint16_t pair = (readMem(sp+1) << 8) | readMem(sp);
	sp += 2;
	return pair;
}