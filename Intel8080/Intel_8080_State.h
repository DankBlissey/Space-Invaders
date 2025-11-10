#pragma once
#include "TestCPU.h"
#include <optional>

struct Intel_8080_State {
    std::optional<uint16_t> pc, sp;
	//std::optional<std::unique_ptr<std::array<uint8_t, 65536>>> mem;
	std::optional<unsigned long> cycles;
	std::optional<uint8_t> B, C, D, E, H, L, A;
	std::optional<bool> Sign, Zero, AuxCarry, Parity, Carry;
	std::optional<std::array<uint8_t, 256>> in, out;

    Intel_8080_State& with_pc(uint16_t in);
	Intel_8080_State& with_sp(uint16_t in);
	//Intel_8080_State& with_mem(std::array<uint8_t, 65536> in)
	Intel_8080_State& with_cycles(unsigned long in);
	Intel_8080_State& with_B(uint8_t in);
	Intel_8080_State& with_C(uint8_t in);
	Intel_8080_State& with_D(uint8_t in);
	Intel_8080_State& with_E(uint8_t in);
	Intel_8080_State& with_H(uint8_t in);
	Intel_8080_State& with_L(uint8_t in);
	Intel_8080_State& with_A(uint8_t in);
	Intel_8080_State& with_Sign(bool in);
	Intel_8080_State& with_Zero(bool in);
	Intel_8080_State& with_AuxCarry(bool in);
	Intel_8080_State& with_Parity(bool in);
	Intel_8080_State& with_Carry(bool in);
	Intel_8080_State& with_in(std::array<uint8_t, 256> ind);
	Intel_8080_State& with_out(std::array<uint8_t, 256> in);

    bool stateEquals(const TestCPU& cpu) const;
};