#include "CPU.h"
#include "Opcode_Cycles.cpp"

// Array of function pointers for each opcode
static void (CPU::* functptr[256])() {
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

CPU::CPU()
	: mem(std::make_unique<std::array<uint8_t, 65536>>()) {
	pc = 0;
	sp = 0;
	// clear input and output buses
	for (int i = 0; i < 256; ++i) {
		in[i] = 0;
		out[i] = 0;
	}
	// Clear memory
	clearMem();
	cycles = 0;
	B = C = D = E = H = L = A = 0;
	Sign = Zero = AuxCarry = Parity = Carry = false;
}

CPU::CPU(const CPU& c)
	: mem(std::make_unique<std::array<uint8_t, 65536>>(*c.mem)) {
	pc = c.pc;
	sp = c.sp;
	in = c.in;
	out = c.out;
	cycles = c.cycles;
	B = c.B;
	C = c.C;
	D = c.D;
	E = c.E;
	H = c.H;
	L = c.L;
	A = c.A;
	Sign = c.Sign;
	Zero = c.Zero;
	AuxCarry = c.AuxCarry;
	Parity = c.Parity;
	Carry = c.Carry;
}

// Initialise the CPU
void CPU::init() {
	pc = 0;
	sp = 0;
	// clear input and output buses
	for (int i = 0; i < 256; ++i) {
		in[i] = 0;
		out[i] = 0;
	}
	// Clear memory
	clearMem();
	cycles = 0;
	B = C = D = E = H = L = A = 0;
	Sign = Zero = AuxCarry = Parity = Carry = false;
}

void CPU::loadTestProgram() {
	// Load a program into memory for testing purposes
	//writeMem(0x0000, 0x37); // STC set carry bit to 1
	//writeMem(0x0001, 0x3F); // CMC complement carry bit to 0
	//writeMem(0x0002, 0x3F); // CMC complement carry bit to 1

	//writeMem(0x0003, 0x14); // INR D increment D register
	H = 0x3A; // Set H to 0x3A
	L = 0x7C; // Set L to 0x7C
	writeMem(0x3A7C, 0x40); // Set memory location 0x3A7C to 0x40
	writeMem(0x0000, 0x35); // DCR M decrement memory location pointed to by HL
}

// Execute a single CPU cycle
void CPU::cycle() {
	// Fetch the opcode
	uint8_t opcode{ readMem(pc) };
	// Execute the opcode
	(this->*functptr[opcode])();
	// Increment the program counter
	pc += 1;
	// Increment the cycle count
	cycles += opcodeCycles[opcode];
}


// Clear memory
void CPU::clearMem() {
	for (int i = 0; i < 65536; ++i)
		(*mem)[i] = 0;
}
// Read a byte from memory
uint8_t CPU::readMem(uint16_t addr) {
	return (*mem)[addr];
}
// Write a byte to memory
void CPU::writeMem(uint16_t addr, uint8_t data) {
	(*mem)[addr] = data;
}
// Read a byte from the input bus
uint8_t CPU::readIn(uint8_t port) {
	return in[port];
}
// Write a byte to the output bus
void CPU::writeOut(uint8_t port, uint8_t data) {
	out[port] = data;
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
// Push a 16-bit value onto the stack
void CPU::stackPush(uint16_t data) {
	(*mem)[sp] = (data & 0xFF00) >> 8;
	(*mem)[sp + 1] = data & 0x00FF;
	sp += 2;
}
// Pop a 16-bit value from the stack
uint16_t CPU::stackPop() {
	sp -= 2;
	return ((*mem)[sp] << 8) | (*mem)[sp + 1];
}


void CPU::status() {
	// Print the status of the CPU registers and flags
	printf("PC: %04X SP: %04X A: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X\n", pc, sp, A, B, C, D, E, H, L);
	printf("Flags - S: %d Z: %d AC: %d P: %d CY: %d\n", Sign, Zero, AuxCarry, Parity, Carry);
	printf("Cycles: %lu\n", cycles);
}

auto CPU::tiedRegisters() const { return std::tie(pc, sp, cycles, B, C, D, E, H, L, A, Sign, Zero, AuxCarry, Parity, Carry); }

bool CPU::operator==(CPU const& rhs) const { 
	return (tiedRegisters() == rhs.tiedRegisters()); 
}