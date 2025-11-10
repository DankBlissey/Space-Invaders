#include "Intel_8080_State.h"

namespace state {
	template <typename T>
	bool matches(const std::optional<T>& opt, const T& val) {
		return !opt.has_value() || opt.value() == val;
	}
}

Intel_8080_State& Intel_8080_State::with_pc(uint16_t in) { pc = in; return *this; }
Intel_8080_State& Intel_8080_State::with_sp(uint16_t in) { sp = in; return *this; }
//Intel_8080_State& Intel_8080_State::with_mem(std::array<uint8_t, 65536> in) { mem = std::make_unique < std::array<uint8_t, 65536>>(in); return *this; }
Intel_8080_State& Intel_8080_State::with_cycles(unsigned long in) { cycles = in; return *this; }
Intel_8080_State& Intel_8080_State::with_B(uint8_t in) { B = in; return *this; }
Intel_8080_State& Intel_8080_State::with_C(uint8_t in) { C = in; return *this; }
Intel_8080_State& Intel_8080_State::with_D(uint8_t in) { D = in; return *this; }
Intel_8080_State& Intel_8080_State::with_E(uint8_t in) { E = in; return *this; }
Intel_8080_State& Intel_8080_State::with_H(uint8_t in) { H = in; return *this; }
Intel_8080_State& Intel_8080_State::with_L(uint8_t in) { L = in; return *this; }
Intel_8080_State& Intel_8080_State::with_A(uint8_t in) { A = in; return *this; }
Intel_8080_State& Intel_8080_State::with_Sign(bool in) { Sign = in; return *this; }
Intel_8080_State& Intel_8080_State::with_Zero(bool in) { Zero = in; return *this; }
Intel_8080_State& Intel_8080_State::with_AuxCarry(bool in) { AuxCarry = in; return *this; }
Intel_8080_State& Intel_8080_State::with_Parity(bool in) { Parity = in; return *this; }
Intel_8080_State& Intel_8080_State::with_Carry(bool in) { Carry = in; return *this; }
Intel_8080_State& Intel_8080_State::with_in(std::array<uint8_t, 256> ind) { in = ind; return *this; }
Intel_8080_State& Intel_8080_State::with_out(std::array<uint8_t, 256> in) { out = in; return *this; }



bool Intel_8080_State::stateEquals(const TestCPU& cpu) const {
	return state::matches(pc, cpu.pc)
		&& state::matches(sp, cpu.sp)
		&& state::matches(cycles, cpu.cycles)
		&& state::matches(B, cpu.B)
		&& state::matches(C, cpu.C)
		&& state::matches(D, cpu.D)
		&& state::matches(E, cpu.E)
		&& state::matches(H, cpu.H)
		&& state::matches(L, cpu.L)
		&& state::matches(A, cpu.A)
		&& state::matches(Sign, cpu.Sign)
		&& state::matches(Zero, cpu.Zero)
		&& state::matches(AuxCarry, cpu.AuxCarry)
		&& state::matches(Parity, cpu.Parity)
		&& state::matches(Carry, cpu.Carry)
		&& state::matches(in, cpu.in)
		&& state::matches(out, cpu.out);
}