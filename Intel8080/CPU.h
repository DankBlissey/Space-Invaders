#pragma once

#include <cstdint>
#include <memory>
#include <array>
//#include <tuple>
//#include <map>

using std::uint8_t;
using std::uint16_t;
using std::int8_t;

class CPU {
	public:
		CPU();
		CPU(const CPU&);
		void init();

		void cycle();					// CPU emulation cycle

		void loadProgram(); // Load a program into memory

		bool operator==(CPU const&) const; // Overwrite == operator to compare the registers of the cpu

	protected:
		void clearMem();
		uint8_t readMem(uint16_t);
		void writeMem(uint16_t, uint8_t);
		uint8_t readIn(uint8_t);
		void writeOut(uint8_t, uint8_t);

		void writePairB(uint16_t);
		uint16_t readPairB();
		void writePairD(uint16_t);
		uint16_t readPairD();
		void writePairH(uint16_t);
		uint16_t readPairH();
		void writePairPSW(uint16_t);
		uint16_t readPairPSW();

		void stackPush(uint16_t);
		uint16_t stackPop();
		
		auto tiedRegisters() const;

		uint16_t pc;					// Program counter
		uint16_t sp;					// Stack pointer
		unsigned long cycles;			// Number of completed cycles
		std::unique_ptr<std::array<uint8_t, 65536>> mem; // 64KB of memory (allocated to heap with unique pointer)
		uint8_t B, C, D, E, H, L, A;	// General purpose registers
		bool Sign, Zero, AuxCarry, 
			Parity, Carry;				// Flags
		std::array<uint8_t, 256> in;	// input bus
		std::array<uint8_t, 256> out;	// Output bus

		using OpFunc = void (CPU::*)();
		static const OpFunc functptr[256];

		// General opcode functions
		void dcr(uint8_t&);
		void inr(uint8_t&);
		void mov(uint8_t&, uint8_t&);
		void add(uint8_t);
		void adc(uint8_t);
		void sub(uint8_t);
		void sbb(uint8_t);
		void ana(uint8_t);
		void xra(uint8_t);
		void ora(uint8_t);
		void cmp(uint8_t);
		void dad(uint16_t); 

		// Specific Opcode functions
		void nop();

		void lxiB();
		void lxiD();
		void lxiH();
		void lxiSP();

		void staxB();
		void staxD();

		void shld();

		void sta();

		void inxB();
		void inxD();
		void inxH();
		void inxSP();

		void inrB();
		void inrD();
		void inrH();
		void inrM();

		void dcrB();
		void dcrD();
		void dcrH();
		void dcrM();

		void mviB();
		void mviD();
		void mviH();
		void mviM();

		void rlc();

		void ral();

		void daa();

		void stc();

		void dadB();
		void dadD();
		void dadH();
		void dadSP();

		void ldaxB();
		void ldaxD();

		void lhld();

		void lda();

		void dcxB();
		void dcxD();
		void dcxH();
		void dcxSP();

		void inrC();
		void inrE();
		void inrL();
		void inrA();

		void dcrC();
		void dcrE();
		void dcrL();
		void dcrA();

		void mviC();
		void mviE();
		void mviL();
		void mviA();

		void rrc();
		void rar();
		void cma();
		void cmc();

		void movBB();
		void movBC();
		void movBD();
		void movBE();
		void movBH();
		void movBL();
		void movBM();
		void movBA();

		void movCB();
		void movCC();
		void movCD();
		void movCE();
		void movCH();
		void movCL();
		void movCM();
		void movCA();

		void movDB();
		void movDC();
		void movDD();
		void movDE();
		void movDH();
		void movDL();
		void movDM();
		void movDA();

		void movEB();
		void movEC();
		void movED();
		void movEE();
		void movEH();
		void movEL();
		void movEM();
		void movEA();

		void movHB();
		void movHC();
		void movHD();
		void movHE();
		void movHH();
		void movHL();
		void movHM();
		void movHA();

		void movLB();
		void movLC();
		void movLD();
		void movLE();
		void movLH();
		void movLL();
		void movLM();
		void movLA();

		void movMB();
		void movMC();
		void movMD();
		void movME();
		void movMH();
		void movML();
		// no movMM, this is hlt instead
		void movMA();

		void movAB();
		void movAC();
		void movAD();
		void movAE();
		void movAH();
		void movAL();
		void movAM();
		void movAA();

		void hlt();

		void addB();
		void addC();
		void addD();
		void addE();
		void addH();
		void addL();
		void addM();
		void addA();

		void adcB();
		void adcC();
		void adcD();
		void adcE();
		void adcH();
		void adcL();
		void adcM();
		void adcA();

		void subB();
		void subC();
		void subD();
		void subE();
		void subH();
		void subL();
		void subM();
		void subA();

		void sbbB();
		void sbbC();
		void sbbD();
		void sbbE();
		void sbbH();
		void sbbL();
		void sbbM();
		void sbbA();

		void anaB();
		void anaC();
		void anaD();
		void anaE();
		void anaH();
		void anaL();
		void anaM();
		void anaA();

		void xraB();
		void xraC();
		void xraD();
		void xraE();
		void xraH();
		void xraL();
		void xraM();
		void xraA();

		void oraB();
		void oraC();
		void oraD();
		void oraE();
		void oraH();
		void oraL();
		void oraM();
		void oraA();

		void cmpB();
		void cmpC();
		void cmpD();
		void cmpE();
		void cmpH();
		void cmpL();
		void cmpM();
		void cmpA();

		void rnz();
		void rnc();
		void rpo();
		void rp();

		void popB();
		void popD();
		void popH();
		void popPSW();

		void jnz();
		void jnc();
		void jpo();
		void jp();

		void jmp();
		void outFunc();
		void xthl();
		void di();

		void cnz();
		void cnc();
		void cpo();
		void cp();

		void pushB();
		void pushD();
		void pushH();
		void pushPSW();

		void adi();
		void sui();
		void ani();
		void ori();

		void rst0();
		void rst2();
		void rst4();
		void rst6();

		void rst1();
		void rst3();
		void rst5();
		void rst7();

		void rz();
		void rc();
		void rpe();
		void rm();

		void ret();
		void pchl();
		
		void sphl();

		void jz();
		void jc();
		void jpe();
		void jm();

		void inFunc();
		void xchg();
		void ei();

		void cz();
		void cc();
		void cpe();
		void cm();

		void call();

		void aci();
		void sbi();
		void xri();
		void cpi();
		// End of opcode functions
};