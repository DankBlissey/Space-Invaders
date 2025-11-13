#include "CPU.h"
#include "Bit_Calcs.cpp"

// General purpose opcode functions are here
void CPU::inr(uint8_t& reg) {
	AuxCarry = calculateAuxCarryINR(reg);
	reg++;
	Parity = checkParity(reg);
	Zero = (reg == 0);
	Sign = checkSign(reg);
}

void CPU::dcr(uint8_t& reg) {
	AuxCarry = calculateAuxCarryDCR(reg);
	reg--;
	Parity = checkParity(reg);
	Zero = (reg == 0);
	Sign = checkSign(reg);
}

void CPU::mov(uint8_t& to, uint8_t& from) {
	to = from;
}

// Specific pcode functions are here
void CPU::nop() {
	// Do nothing
	// I mean this opcode actually, literally does nothing
}

void CPU::lxiB() {
	// Do nothing
}

void CPU::lxiD() {
	// Do nothing
}

void CPU::lxiH() {
	// Do nothing
}

void CPU::lxiSP() {
	// Do nothing
}

void CPU::staxB() {
	writeMem(readPairB(), A);
}

void CPU::staxD() {
	writeMem(readPairD(), A);
}

void CPU::shld() {
	// Do nothing
}

void CPU::sta() {
	// Do nothing
}

void CPU::inxB() {
	// Do nothing
}

void CPU::inxD() {
	// Do nothing
}

void CPU::inxH() {
	// Do nothing
}

void CPU::inxSP() {
	// Do nothing
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
	// Do nothing
}

void CPU::mviD() {
	// Do nothing
}

void CPU::mviH() {
	// Do nothing
}

void CPU::mviM() {
	// Do nothing
}

void CPU::rlc() {
	// Do nothing
}

void CPU::ral() {
	// Do nothing
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
	// Do nothing
}

void CPU::dadD() {
	// Do nothing
}

void CPU::dadH() {
	// Do nothing
}

void CPU::dadSP() {
	// Do nothing
}

void CPU::ldaxB() {
	A = readMem(readPairB());
}

void CPU::ldaxD() {
	A = readMem(readPairH());
}

void CPU::lhld() {
	// Do nothing
}

void CPU::lda() {
	// Do nothing
}

void CPU::dcxB() {
	// Do nothing
}

void CPU::dcxD() {
	// Do nothing
}

void CPU::dcxH() {
	// Do nothing
}

void CPU::dcxSP() {
	// Do nothing
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
	// Do nothing
}

void CPU::mviE() {
	// Do nothing
}

void CPU::mviL() {
	// Do nothing
}

void CPU::mviA() {
	// Do nothing
}

void CPU::rrc() {
	// Do nothing
}

void CPU::rar() {
	// Do nothing
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
	// Do nothing
}

void CPU::addB() {
	// Do nothing
}

void CPU::addC() {
	// Do nothing
}

void CPU::addD() {
	// Do nothing
}

void CPU::addE() {
	// Do nothing
}

void CPU::addH() {
	// Do nothing
}

void CPU::addL() {
	// Do nothing
}

void CPU::addM() {
	// Do nothing
}

void CPU::addA() {
	// Do nothing
}

void CPU::adcB() {
	// Do nothing
}

void CPU::adcC() {
	// Do nothing
}

void CPU::adcD() {
	// Do nothing
}

void CPU::adcE() {
	// Do nothing
}

void CPU::adcH() {
	// Do nothing
}

void CPU::adcL() {
	// Do nothing
}

void CPU::adcM() {
	// Do nothing
}

void CPU::adcA() {
	// Do nothing
}

void CPU::subB() {
	// Do nothing
}

void CPU::subC() {
	// Do nothing
}

void CPU::subD() {
	// Do nothing
}

void CPU::subE() {
	// Do nothing
}

void CPU::subH() {
	// Do nothing
}

void CPU::subL() {
	// Do nothing
}

void CPU::subM() {
	// Do nothing
}

void CPU::subA() {
	// Do nothing
}

void CPU::sbbB() {
	// Do nothing
}

void CPU::sbbC() {
	// Do nothing
}

void CPU::sbbD() {
	// Do nothing
}

void CPU::sbbE() {
	// Do nothing
}

void CPU::sbbH() {
	// Do nothing
}

void CPU::sbbL() {
	// Do nothing
}

void CPU::sbbM() {
	// Do nothing
}

void CPU::sbbA() {
	// Do nothing
}

void CPU::anaB() {
	// Do nothing
}

void CPU::anaC() {
	// Do nothing
}

void CPU::anaD() {
	// Do nothing
}

void CPU::anaE() {
	// Do nothing
}

void CPU::anaH() {
	// Do nothing
}

void CPU::anaL() {
	// Do nothing
}

void CPU::anaM() {
	// Do nothing
}

void CPU::anaA() {
	// Do nothing
}

void CPU::xraB() {
	// Do nothing
}

void CPU::xraC() {
	// Do nothing
}

void CPU::xraD() {
	// Do nothing
}

void CPU::xraE() {
	// Do nothing
}

void CPU::xraH() {
	// Do nothing
}

void CPU::xraL() {
	// Do nothing
}

void CPU::xraM() {
	// Do nothing
}

void CPU::xraA() {
	// Do nothing
}

void CPU::oraB() {
	// Do nothing
}

void CPU::oraC() {
	// Do nothing
}

void CPU::oraD() {
	// Do nothing
}

void CPU::oraE() {
	// Do nothing
}

void CPU::oraH() {
	// Do nothing
}

void CPU::oraL() {
	// Do nothing
}

void CPU::oraM() {
	// Do nothing
}

void CPU::oraA() {
	// Do nothing
}

void CPU::cmpB() {
	// Do nothing
}

void CPU::cmpC() {
	// Do nothing
}

void CPU::cmpD() {
	// Do nothing
}

void CPU::cmpE() {
	// Do nothing
}

void CPU::cmpH() {
	// Do nothing
}

void CPU::cmpL() {
	// Do nothing
}

void CPU::cmpM() {
	// Do nothing
}

void CPU::cmpA() {
	// Do nothing
}

void CPU::rnz() {
	// Do nothing
}

void CPU::rnc() {
	// Do nothing
}

void CPU::rpo() {
	// Do nothing
}

void CPU::rp() {
	// Do nothing
}

void CPU::popB() {
	// Do nothing
}

void CPU::popD() {
	// Do nothing
}

void CPU::popH() {
	// Do nothing
}

void CPU::popPSW() {
	// Do nothing
}

void CPU::jnz() {
	// Do nothing
}

void CPU::jnc() {
	// Do nothing
}

void CPU::jpo() {
	// Do nothing
}

void CPU::jp() {
	// Do nothing
}

void CPU::jmp() {
	// Do nothing
}

void CPU::outFunc() {
	// Do nothing
}

void CPU::xthl() {
	// Do nothing
}

void CPU::di() {
	// Do nothing
}

void CPU::cnz() {
	// Do nothing
}

void CPU::cnc() {
	// Do nothing
}

void CPU::cpo() {
	// Do nothing
}

void CPU::cp() {
	// Do nothing
}

void CPU::pushB() {
	// Do nothing
}

void CPU::pushD() {
	// Do nothing
}

void CPU::pushH() {
	// Do nothing
}

void CPU::pushPSW() {
	// Do nothing
}

void CPU::adi() {
	// Do nothing
}

void CPU::sui() {
	// Do nothing
}

void CPU::ani() {
	// Do nothing
}

void CPU::ori() {
	// Do nothing
}

void CPU::rst0() {
	// Do nothing
}

void CPU::rst2() {
	// Do nothing
}

void CPU::rst4() {
	// Do nothing
}

void CPU::rst6() {
	// Do nothing
}

void CPU::rst1() {
	// Do nothing
}

void CPU::rst3() {
	// Do nothing
}

void CPU::rst5() {
	// Do nothing
}

void CPU::rst7() {
	// Do nothing
}

void CPU::rz() {
	// Do nothing
}

void CPU::rc() {
	// Do nothing
}

void CPU::rpe() {
	// Do nothing
}

void CPU::rm() {
	// Do nothing
}

void CPU::ret() {
	// Do nothing
}

void CPU::pchl() {
	// Do nothing
}

void CPU::sphl() {
	// Do nothing
}

void CPU::jz() {
	// Do nothing
}

void CPU::jc() {
	// Do nothing
}

void CPU::jpe() {
	// Do nothing
}

void CPU::jm() {
	// Do nothing
}

void CPU::inFunc() {
	// Do nothing
}

void CPU::xchg() {
	// Do nothing
}

void CPU::ei() {
	// Do nothing
}

void CPU::cz() {
	// Do nothing
}

void CPU::cc() {
	// Do nothing
}

void CPU::cpe() {
	// Do nothing
}

void CPU::cm() {
	// Do nothing
}

void CPU::call() {
	// Do nothing
}

void CPU::aci() {
	// Do nothing
}

void CPU::sbi() {
	// Do nothing
}

void CPU::xri() {
	// Do nothing
}

void CPU::cpi() {
	// Do nothing
}

// End of opcode functions