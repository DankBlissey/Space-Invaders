#include "CPU.h"
#include <optional>

struct Intel_8080_State {
	std::optional<uint16_t> pc, sp;
	std::optional<std::unique_ptr<std::array<uint8_t, 65536>>> mem;
	std::optional<unsigned long> cycles;
	std::optional<uint8_t> B, C, D, E, H, L, A;
	std::optional<bool> Sign, Zero, AuxCarry, Parity, Carry;
	std::optional<std::array<uint8_t, 256>> in, out;

	Intel_8080_State& with_pc(uint16_t in) { pc = in; return *this; }
	Intel_8080_State& with_sp(uint16_t in) { sp = in; return *this; }
	Intel_8080_State& with_mem(std::array<uint8_t, 65536> in) { mem = std::make_unique < std::array<uint8_t, 65536>>(in); return *this; }
	Intel_8080_State& with_cycles(unsigned long in) { cycles = in; return *this; }
	Intel_8080_State& with_B(uint8_t in) { B = in; return *this; }
	Intel_8080_State& with_C(uint8_t in) { C = in; return *this; }
	Intel_8080_State& with_D(uint8_t in) { D = in; return *this; }
	Intel_8080_State& with_E(uint8_t in) { E = in; return *this; }
	Intel_8080_State& with_H(uint8_t in) { H = in; return *this; }
	Intel_8080_State& with_L(uint8_t in) { L = in; return *this; }
	Intel_8080_State& with_A(uint8_t in) { A = in; return *this; }
	Intel_8080_State& with_Sign(bool in) { Sign = in; return *this; }
	Intel_8080_State& with_Zero(bool in) { Zero = in; return *this; }
	Intel_8080_State& with_AuxCarry(bool in) { AuxCarry = in; return *this; }
	Intel_8080_State& with_Parity(bool in) { Parity = in; return *this; }
	Intel_8080_State& with_Carry(bool in) { Carry = in; return *this; }
	Intel_8080_State& with_in(std::array<uint8_t, 256> ind) { in = ind; return *this; }
	Intel_8080_State& with_out(std::array<uint8_t, 256> in) { out = in; return *this; }

	bool equals(CPU& cpu) const {
		if (pc && cpu.pc != *pc) return false;
		if (sp && cpu.sp != *sp) return false;
		if (cycles && cpu.cycles != *cycles) return false;
		if (B && cpu.getB() != *B) return false;
		return true;
	}
};