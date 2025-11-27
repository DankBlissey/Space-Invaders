#include "TestCPU.h"
#include <iostream>
#include "OpcodeTableValues.cpp"

// Getter functions
uint16_t TestCPU:: getPc() {
    return pc;
}

uint16_t TestCPU:: getSp() {
    return sp;
}

uint8_t TestCPU::getExtraCycles() {
    return extraCycles;
}

std::ostream& operator<<(std::ostream& os, const TestCPU& cpu) {
    os << "-------------------------CPU State --------------------------\n"
        << "PC: " << std::hex << static_cast<int>(cpu.pc) << ", SP: " << std::hex << static_cast<int>(cpu.sp) << "\n"
        << "Next Instruction: " << std::hex << static_cast<int>(cpu.readMem(cpu.pc)) << "\n"
        << "Instruction done: Opcode: " << std::hex << static_cast<int>(cpu.currentInstruction) << ", Operand bytes: ";
    if (cpu.instructionSecondByte.has_value()) {
        os << static_cast<int>(cpu.instructionSecondByte.value());
    } else {
        os << "Nothing";
    }
    os << ", ";
    if (cpu.instructionThirdByte.has_value()) {
        os << static_cast<int>(cpu.instructionThirdByte.value());
    } else {
        os << "Nothing";
    }
    os << "\n";
    os  << "Register B: " << std::hex << static_cast<int>(cpu.B) << "\n"
        << "Register C: " << std::hex << static_cast<int>(cpu.C) << "\n"
        << "Register D: " << std::hex << static_cast<int>(cpu.D) << "\n"
        << "Register E: " << std::hex << static_cast<int>(cpu.E) << "\n"
        << "Register H: " << std::hex << static_cast<int>(cpu.H) << "\n"
        << "Register L: " << std::hex << static_cast<int>(cpu.L) << "\n"
        << "Accumulator: " << std::hex << static_cast<int>(cpu.A) << "\n"
        << "Carry: " << std::hex << static_cast<int>(cpu.Carry) << "\n"
        << "AuxCarry: " << std::hex << static_cast<int>(cpu.AuxCarry) << "\n"
        << "Sign: " << std::hex << static_cast<int>(cpu.Sign) << "\n"
        << "Parity: " << std::hex << static_cast<int>(cpu.Parity) << "\n"
        << "Zero: " << std::hex << static_cast<int>(cpu.Zero) << "\n";
    return os;
}

uint8_t TestCPU::getMem(uint16_t addr) {
    return readMem(addr);
}

uint16_t TestCPU::getPairB() {
    return readPairB();
}

uint16_t TestCPU::getPairD() {
    return readPairD();
}

uint16_t TestCPU::getPairH() {
    return readPairH();
}

uint16_t TestCPU::getPairPSW() {
    return readPairPSW();
}

uint8_t TestCPU::getB() {
    return B;
}

uint8_t TestCPU::getC() {
    return C;
}

uint8_t TestCPU::getD() {
    return D;
}

uint8_t TestCPU::getE() {
    return E;
}

uint8_t TestCPU::getH() {
    return H;
}

uint8_t TestCPU::getL() {
    return L;
}

uint8_t TestCPU::getA() {
    return A;
}

bool TestCPU::getSign() {
    return Sign;
}

bool TestCPU::getZero() {
    return Zero;
}

bool TestCPU::getAuxCarry() {
    return AuxCarry;
}

bool TestCPU::getParity() {
    return Parity;
}

bool TestCPU::getCarry() {
    return Carry;
}

// Setter functions
void TestCPU::setPc(uint16_t in) {
    pc = in;
}

void TestCPU::setSp(uint16_t in) {
    sp = in;
}

void TestCPU::setExtraCycles(uint8_t in) {
    extraCycles = in;
}

void TestCPU::setMem(uint16_t addr, uint8_t data) {
    writeMem(addr, data);
}

void TestCPU::setPairB(uint16_t data) {
    writePairB(data);
}

void TestCPU::setPairD(uint16_t data) {
    writePairD(data);
}

void TestCPU::setPairH(uint16_t data) {
    writePairH(data);
}

void TestCPU::setPairPSW(uint16_t data) {
    writePairPSW(data);
}

void TestCPU::setB(uint8_t in) {
    B = in;
}

void TestCPU::setC(uint8_t in) {
    C = in;
}

void TestCPU::setD(uint8_t in) {
    D = in;
}

void TestCPU::setE(uint8_t in) {
    E = in;
}

void TestCPU::setH(uint8_t in) {
    H = in;
}

void TestCPU::setL(uint8_t in) {
    L = in;
}

void TestCPU::setA(uint8_t in) {
    A = in;
}

void TestCPU::setSign(bool in) {
    Sign = in;
}

void TestCPU::setZero(bool in) {
    Zero = in;
}

void TestCPU::setAuxCarry(bool in) {
    AuxCarry = in;
}

void TestCPU::setParity(bool in) {
    Parity = in;
}

void TestCPU::setCarry(bool in) {
    Carry = in;
}

void TestCPU::setAllReg(uint8_t in) {
    setB(in);
    setC(in);
    setD(in);
    setE(in);
    setH(in);
    setL(in);
    setA(in);
}


uint8_t TestCPU::cycle() {
    uint8_t cycles = CPU::cycle();
    switch(opcodeByteLength[currentInstruction]) {
        case 1:
            instructionSecondByte.reset();
            instructionThirdByte.reset();
            break;
        case 2:
            instructionSecondByte = readMem(pc-1);
            instructionThirdByte.reset();
            break;
        case 3:
            instructionSecondByte = readMem(pc-2);
            instructionThirdByte = readMem(pc-1);
            break;
    }
    return cycles;
}