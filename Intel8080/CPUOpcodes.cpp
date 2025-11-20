#include "CPU.h"
#include <algorithm>

// Helper functions

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

// Checks if byte is zero
static bool checkZero(uint8_t value) {
	return (value == 0);
}

// Functions to calculate the carry and auxiliary carry flags
static bool calculateAuxCarryINR(uint8_t value) {
	return ((value & 0x0F) + 1) > 0x0F;
}

static bool calculateAuxCarryDCR(uint8_t value) {
	return (value & 0x0F) == 0;
}

static bool calculateAuxCarryADD(uint8_t a, uint8_t b) {
	return ((a & 0x0F) + (b & 0x0F)) > 0x0F;
}

static bool calculateCarryADD(uint8_t a, uint8_t b) {
	return (uint16_t)a + (uint16_t)b > 0xFF;
}

static uint8_t twosComplement(uint8_t value) {
	return ~value + 1;
}

static bool calculateCarryDAD(uint16_t a, uint16_t b) {
	return (unsigned long)a + (unsigned long)b > 0xFFFF;
}

static uint16_t combineBytes(uint8_t most, uint16_t least) {
	return (most << 8) | least;
}



// General purpose opcode functions are here
void CPU::inr(uint8_t& reg) {
	AuxCarry = calculateAuxCarryINR(reg);
	reg++;
	Parity = checkParity(reg);
	Zero = checkZero(reg);
	Sign = checkSign(reg);
}

void CPU::dcr(uint8_t& reg) {
	AuxCarry = calculateAuxCarryDCR(reg);
	reg--;
	Parity = checkParity(reg);
	Zero = checkZero(reg);
	Sign = checkSign(reg);
}

void CPU::mov(uint8_t& to, uint8_t& from) {
	to = from;
}

void CPU::add(uint8_t reg) {
	AuxCarry = calculateAuxCarryADD(A, reg);
	Carry = calculateCarryADD(A, reg);
	A += reg;
	Parity = checkParity(A);
	Zero = checkZero(A);
	Sign = checkSign(A);
}

void CPU::adc(uint8_t reg) {
	if (Carry) {
		reg++;
	}
	AuxCarry = calculateAuxCarryADD(A, reg);
	Carry = calculateCarryADD(A, reg);
	A += reg;
	Parity = checkParity(A);
	Zero = checkZero(A);
	Sign = checkSign(A);
}

void CPU::sub(uint8_t reg) {
	reg = twosComplement(reg);
	AuxCarry = calculateAuxCarryADD(A, reg);
	Carry = !calculateCarryADD(A, reg);
	A += reg;
	Parity = checkParity(A);
	Zero = checkZero(A);
	Sign = checkSign(A);
}

void CPU::sbb(uint8_t reg) {
	if (Carry) {
		reg++;
	}
	reg = twosComplement(reg);
	AuxCarry = calculateAuxCarryADD(A, reg);
	Carry = !calculateCarryADD(A, reg);
	A += reg;
	Parity = checkParity(A);
	Zero = checkZero(A);
	Sign = checkSign(A);
}

void CPU::ana(uint8_t reg) {
	A = A & reg;
	Carry = false;
	Parity = checkParity(A);
	Zero = checkZero(A);
	Sign = checkSign(A);
}

void CPU::xra(uint8_t reg) {
	A = A ^ reg;
	Carry = false;
	AuxCarry = false;
	Parity = checkParity(A);
	Zero = checkZero(A);
	Sign = checkSign(A);
}

void CPU::ora(uint8_t reg) {
	A = A | reg;
	Carry = false;
	Parity = checkParity(A);
	Zero = checkZero(A);
	Sign = checkSign(A);
}

void CPU::cmp(uint8_t reg) {
	uint8_t negativeReg = twosComplement(reg);
	uint8_t subValue = A + negativeReg;
	AuxCarry = calculateAuxCarryADD(A, negativeReg);
	Carry = !calculateCarryADD(A,negativeReg);
	Sign = checkSign(subValue);
	Parity = checkParity(subValue);
	Zero = checkZero(subValue);
}

void CPU::dad(uint16_t reg) {
	uint16_t pair {readPairH()};
	Carry = calculateCarryDAD(pair, reg);
	writePairH(pair + reg);
}

void CPU::mvi(uint8_t& reg) {
	reg = readMem(pc-1);
}

uint16_t CPU::readImmediate() {
	return combineBytes(readMem(pc - 1), readMem(pc - 2));
}

void CPU::conditionalJump(bool flag) {
	if (flag) {
		pc = readImmediate();
	}
}

void CPU::callJump() {
	stackPush(pc);
	pc = readImmediate();
}

void CPU::conditionalCallJump(bool flag) {
	if (flag) {
		callJump();
	}
}

void CPU::returnFromCall() {
	pc = stackPop();
}

void CPU::conditionalReturnFromCall(bool flag) {
	if (flag) {
		returnFromCall();
	}
}

void CPU::rst(uint16_t exp) {
	stackPush(pc);
	pc = exp << 3;
}

// Specific opcode functions only below here

void CPU::nop() {
	// Do nothing
	// I mean this opcode actually, literally does nothing
}

void CPU::lxiB() {
	B = readMem(pc-1);
	C = readMem(pc-2);
}

void CPU::lxiD() {
	D = readMem(pc-1);
	E = readMem(pc-2);
}

void CPU::lxiH() {
	H = readMem(pc-1);
	L = readMem(pc-2);
}

void CPU::lxiSP() {
	sp = readImmediate();
}

void CPU::staxB() {
	writeMem(readPairB(), A);
}

void CPU::staxD() {
	writeMem(readPairD(), A);
}

void CPU::shld() {
	uint16_t addr {readImmediate()};
	writeMem(addr, L);
	writeMem(addr + 1, H);
}

void CPU::sta() {
	writeMem(readImmediate(), A);
}

void CPU::inxB() {
	writePairB(readPairB() + 1);
}

void CPU::inxD() {
	writePairD(readPairD() + 1);
}

void CPU::inxH() {
	writePairH(readPairH() + 1);
}

void CPU::inxSP() {
	sp++;
}

void CPU::inrB() {
	inr(B);
}

void CPU::inrD() {
	inr(D);
}

void CPU::inrH() {
	inr(H);
}

void CPU::inrM() {
	uint16_t location{ readPairH() };
	uint8_t data{ readMem(location) };
	AuxCarry = calculateAuxCarryINR(data);
	data++;
	writeMem(location, data);
	Parity = checkParity(data);
	Zero = (data == 0);
	Sign = checkSign(data);
}

void CPU::dcrB() {
	dcr(B);
}

void CPU::dcrD() {
	dcr(D);
}

void CPU::dcrH() {
	dcr(H);
}

void CPU::dcrM() {
	uint16_t location{ readPairH() };
	uint8_t data{ readMem(location) };
	AuxCarry = calculateAuxCarryDCR(data);
	data--;
	writeMem(location, data);
	Parity = checkParity(data);
	Zero = (data == 0);
	Sign = checkSign(data);
}

void CPU::mviB() {
	mvi(B);
}

void CPU::mviD() {
	mvi(D);
}

void CPU::mviH() {
	mvi(H);
}

void CPU::mviM() {
	writeMem(readPairH(), readMem(pc-1));
}

void CPU::rlc() {
	Carry = checkSign(A);
	uint8_t temp = A << 1;
	if(Carry) {
		temp = temp | 0x01;
	}
	A = temp;
}

void CPU::ral() {
	uint8_t temp = A << 1;
	if(Carry) {
		temp = temp | 0x01;
	}
	Carry = checkSign(A);
	A = temp;
}

void CPU::daa() {
	if ((A & 0x0F) > 9 || AuxCarry) {
		uint8_t oldA{ A };
		A += 0x06;
		AuxCarry = calculateAuxCarryADD(oldA, 0x06);
	}
	if ((A & 0xF0) > 0x90 || Carry) {
		uint8_t oldA{ A };
		A += 0x60;
		Carry = calculateCarryADD(oldA, 0x60);
	}
	Parity = checkParity(A);
	Zero = (A == 0);
	Sign = checkSign(A);
}

void CPU::stc() {
	Carry = true;
}

void CPU::dadB() {
	dad(readPairB());
}

void CPU::dadD() {
	dad(readPairD());
}

void CPU::dadH() {
	dad(readPairH());
}

void CPU::dadSP() {
	dad(sp);
}

void CPU::ldaxB() {
	A = readMem(readPairB());
}

void CPU::ldaxD() {
	A = readMem(readPairD());
}

void CPU::lhld() {
	uint16_t addr {readImmediate()};
	L = readMem(addr);
	H = readMem(addr + 1);
}

void CPU::lda() {
	A = readMem(readImmediate());
}

void CPU::dcxB() {
	writePairB(readPairB() - 1);
}

void CPU::dcxD() {
	writePairD(readPairD() - 1);
}

void CPU::dcxH() {
	writePairH(readPairH() - 1);
}

void CPU::dcxSP() {
	sp--;
}

void CPU::inrC() {
	inr(C);
}

void CPU::inrE() {
	inr(E);
}

void CPU::inrL() {
	inr(L);
}

void CPU::inrA() {
	inr(A);
}

void CPU::dcrC() {
	dcr(C);
}

void CPU::dcrE() {
	dcr(E);
}

void CPU::dcrL() {
	dcr(L);
}

void CPU::dcrA() {
	dcr(A);
}

void CPU::mviC() {
	mvi(C);
}

void CPU::mviE() {
	mvi(E);
}

void CPU::mviL() {
	mvi(L);
}

void CPU::mviA() {
	mvi(A);
}

void CPU::rrc() {
	Carry = (A & 0x01) != 0;
	uint8_t temp = A >> 1;
	if(Carry) {
		temp = temp | 0x80;
	}
	A = temp;
}

void CPU::rar() {
	uint8_t temp = A >> 1;
	if(Carry) {
		temp = temp | 0x80;
	}
	Carry = (A & 0x01) != 0;
	A = temp;
}

void CPU::cma() {
	A = ~A; // Complement the bits held in the Accumulator
}

void CPU::cmc() {
	Carry = !Carry;
}

void CPU::movBB() {
	// Do nothing
}

void CPU::movBC() {
	mov(B,C);
}

void CPU::movBD() {
	mov(B,D);
}

void CPU::movBE() {
	mov(B,E);
}

void CPU::movBH() {
	mov(B,H);
}

void CPU::movBL() {
	mov(B,L);
}

void CPU::movBM() {
	B = readMem(readPairH());
}

void CPU::movBA() {
	mov(B,A);
}

void CPU::movCB() {
	mov(C,B);
}

void CPU::movCC() {
	// Do nothing
}

void CPU::movCD() {
	mov(C,D);
}

void CPU::movCE() {
	mov(C,E);
}

void CPU::movCH() {
	mov(C,H);
}

void CPU::movCL() {
	mov(C,L);
}

void CPU::movCM() {
	C = readMem(readPairH());
}

void CPU::movCA() {
	mov(C,A);
}

void CPU::movDB() {
	mov(D,B);
}

void CPU::movDC() {
	mov(D,C);
}

void CPU::movDD() {
	// Do nothing
}

void CPU::movDE() {
	mov(D,E);
}

void CPU::movDH() {
	mov(D,H);
}

void CPU::movDL() {
	mov(D,L);
}

void CPU::movDM() {
	D = readMem(readPairH());
}

void CPU::movDA() {
	mov(D,A);
}

void CPU::movEB() {
	mov(E,B);
}

void CPU::movEC() {
	mov(E,C);
}

void CPU::movED() {
	mov(E,D);
}

void CPU::movEE() {
	// Do nothing
}

void CPU::movEH() {
	mov(E,H);
}

void CPU::movEL() {
	mov(E,L);
}

void CPU::movEM() {
	E = readMem(readPairH());
}

void CPU::movEA() {
	mov(E,A);
}

void CPU::movHB() {
	mov(H,B);
}

void CPU::movHC() {
	mov(H,C);
}

void CPU::movHD() {
	mov(H,D);
}

void CPU::movHE() {
	mov(H,E);
}

void CPU::movHH() {
	// Do nothing
}

void CPU::movHL() {
	mov(H,L);
}

void CPU::movHM() {
	H = readMem(readPairH());
}

void CPU::movHA() {
	mov(H,A);
}

void CPU::movLB() {
	mov(L,B);
}

void CPU::movLC() {
	mov(L,C);
}

void CPU::movLD() {
	mov(L,D);
}

void CPU::movLE() {
	mov(L,E);
}

void CPU::movLH() {
	mov(L,H);
}

void CPU::movLL() {
	// Do nothing
}

void CPU::movLM() {
	L = readMem(readPairH());
}

void CPU::movLA() {
	mov(L,A);
}

void CPU::movMB() {
	writeMem(readPairH(), B);
}

void CPU::movMC() {
	writeMem(readPairH(), C);
}

void CPU::movMD() {
	writeMem(readPairH(), D);
}

void CPU::movME() {
	writeMem(readPairH(), E);
}

void CPU::movMH() {
	writeMem(readPairH(), H);
}

void CPU::movML() {
	writeMem(readPairH(), L);
}

void CPU::movMA() {
	writeMem(readPairH(), A);
}

void CPU::movAB() {
	mov(A,B);
}

void CPU::movAC() {
	mov(A,C);
}

void CPU::movAD() {
	mov(A,D);
}

void CPU::movAE() {
	mov(A,E);
}

void CPU::movAH() {
	mov(A,H);
}

void CPU::movAL() {
	mov(A,L);
}

void CPU::movAM() {
	A = readMem(readPairH());
}

void CPU::movAA() {
	// Do nothing
}

void CPU::hlt() {
	STOPPED = true;
}

void CPU::addB() {
	add(B);
}

void CPU::addC() {
	add(C);
}

void CPU::addD() {
	add(D);
}

void CPU::addE() {
	add(E);
}

void CPU::addH() {
	add(H);
}

void CPU::addL() {
	add(L);
}

void CPU::addM() {
	add(readMem(readPairH()));
}

void CPU::addA() {
	add(A);
}

void CPU::adcB() {
	adc(B);
}

void CPU::adcC() {
	adc(C);
}

void CPU::adcD() {
	adc(D);
}

void CPU::adcE() {
	adc(E);
}

void CPU::adcH() {
	adc(H);
}

void CPU::adcL() {
	adc(L);
}

void CPU::adcM() {
	adc(readMem(readPairH()));
}

void CPU::adcA() {
	adc(A);
}

void CPU::subB() {
	sub(B);
}

void CPU::subC() {
	sub(C);
}

void CPU::subD() {
	sub(D);
}

void CPU::subE() {
	sub(E);
}

void CPU::subH() {
	sub(H);
}

void CPU::subL() {
	sub(L);
}

void CPU::subM() {
	sub(readMem(readPairH()));
}

void CPU::subA() {
	sub(A);
}

void CPU::sbbB() {
	sbb(B);
}

void CPU::sbbC() {
	sbb(C);
}

void CPU::sbbD() {
	sbb(D);
}

void CPU::sbbE() {
	sbb(E);
}

void CPU::sbbH() {
	sbb(H);
}

void CPU::sbbL() {
	sbb(L);
}

void CPU::sbbM() {
	sbb(readMem(readPairH()));
}

void CPU::sbbA() {
	sbb(A);
}

void CPU::anaB() {
	ana(B);
}

void CPU::anaC() {
	ana(C);
}

void CPU::anaD() {
	ana(D);
}

void CPU::anaE() {
	ana(E);
}

void CPU::anaH() {
	ana(H);
}

void CPU::anaL() {
	ana(L);
}

void CPU::anaM() {
	ana(readMem(readPairH()));
}

void CPU::anaA() {
	ana(A);
}

void CPU::xraB() {
	xra(B);
}

void CPU::xraC() {
	xra(C);
}

void CPU::xraD() {
	xra(D);
}

void CPU::xraE() {
	xra(E);
}

void CPU::xraH() {
	xra(H);
}

void CPU::xraL() {
	xra(L);
}

void CPU::xraM() {
	xra(readMem(readPairH()));
}

void CPU::xraA() {
	xra(A);
}

void CPU::oraB() {
	ora(B);
}

void CPU::oraC() {
	ora(C);
}

void CPU::oraD() {
	ora(D);
}

void CPU::oraE() {
	ora(E);
}

void CPU::oraH() {
	ora(H);
}

void CPU::oraL() {
	ora(L);
}

void CPU::oraM() {
	ora(readMem(readPairH()));
}

void CPU::oraA() {
	ora(A);
}

void CPU::cmpB() {
	cmp(B);
}

void CPU::cmpC() {
	cmp(C);
}

void CPU::cmpD() {
	cmpD();
}

void CPU::cmpE() {
	cmp(E);
}

void CPU::cmpH() {
	cmp(H);
}

void CPU::cmpL() {
	cmp(L);
}

void CPU::cmpM() {
	cmp(readMem(readPairH()));
}

void CPU::cmpA() {
	cmp(A);
}

void CPU::rnz() {
	conditionalReturnFromCall(!Zero);
}

void CPU::rnc() {
	conditionalReturnFromCall(!Carry);
}

void CPU::rpo() {
	conditionalReturnFromCall(!Parity);
}

void CPU::rp() {
	conditionalReturnFromCall(!Sign);
}

void CPU::popB() {
	writePairB(stackPop());
}

void CPU::popD() {
	writePairD(stackPop());
}

void CPU::popH() {
	writePairH(stackPop());
}

void CPU::popPSW() {
	writePairPSW(stackPop());
}

void CPU::jnz() {
	conditionalJump(!Zero);
}

void CPU::jnc() {
	conditionalJump(!Carry);
}

void CPU::jpo() {
	conditionalJump(!Parity);
}

void CPU::jp() {
	conditionalJump(!Sign);
}

void CPU::jmp() {
	pc = readImmediate();
}

void CPU::outFunc() {
	writeOut(readMem(pc-1), A);
}

void CPU::xthl() {
	uint8_t oldL {L};
	uint8_t oldH {H};
	L = readMem(sp);
	H = readMem(sp + 1);
	writeMem(sp, oldL);
	writeMem(sp + 1, oldH);
}

void CPU::di() {
	INTE = false;
}

void CPU::cnz() {
	conditionalCallJump(!Zero);
}

void CPU::cnc() {
	conditionalCallJump(!Carry);
}

void CPU::cpo() {
	conditionalCallJump(!Parity);
}

void CPU::cp() {
	conditionalCallJump(!Sign);
}

void CPU::pushB() {
	stackPush(readPairB());
}

void CPU::pushD() {
	stackPush(readPairD());
}

void CPU::pushH() {
	stackPush(readPairH());
}

void CPU::pushPSW() {
	stackPush(readPairPSW());
}

void CPU::adi() {
	add(readMem(pc-1));
}

void CPU::sui() {
	sub(readMem(pc-1));
}

void CPU::ani() {
	ana(readMem(pc-1));
}

void CPU::ori() {
	ora(readMem(pc-1));
}

void CPU::rst0() {
	rst(0);
}

void CPU::rst2() {
	rst(2);
}

void CPU::rst4() {
	rst(4);
}

void CPU::rst6() {
	rst(6);
}

void CPU::rst1() {
	rst(1);
}

void CPU::rst3() {
	rst(3);
}

void CPU::rst5() {
	rst(5);
}

void CPU::rst7() {
	rst(7);
}

void CPU::rz() {
	conditionalReturnFromCall(Zero);
}

void CPU::rc() {
	conditionalReturnFromCall(Carry);
}

void CPU::rpe() {
	conditionalReturnFromCall(Parity);
}

void CPU::rm() {
	conditionalReturnFromCall(Sign);
}

void CPU::ret() {
	returnFromCall();
}

void CPU::pchl() {
	pc = combineBytes(H,L);
}

void CPU::sphl() {
	sp = readPairH();
}

void CPU::jz() {
	conditionalJump(Zero);
}

void CPU::jc() {
	conditionalJump(Carry);
}

void CPU::jpe() {
	conditionalJump(Parity);
}

void CPU::jm() {
	conditionalJump(Sign);
}

void CPU::inFunc() {
	A = readIn(readMem(pc-1));
}

void CPU::xchg() {
	std::swap(D,H);
	std::swap(E,L);
}

void CPU::ei() {
	INTE = true;
}

void CPU::cz() {
	conditionalCallJump(Zero);
}

void CPU::cc() {
	conditionalCallJump(Carry);
}

void CPU::cpe() {
	conditionalCallJump(Parity);
}

void CPU::cm() {
	conditionalCallJump(Sign);
}

void CPU::call() {
	callJump();
}

void CPU::aci() {
	adc(readMem(pc-1));
}

void CPU::sbi() {
	sbb(readMem(pc-1));
}

void CPU::xri() {
	xra(readMem(pc-1));
}

void CPU::cpi() {
	cmp(readMem(pc-1));
}

// End of opcode functions