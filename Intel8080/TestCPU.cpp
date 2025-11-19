#include "TestCPU.h"

// Getter functions
uint16_t TestCPU:: getPc() {
    return pc;
}

uint16_t TestCPU:: getSp() {
    return sp;
}

unsigned long TestCPU::getCycles() {
    return cycles;
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

void TestCPU::setCycles(unsigned long in) {
    cycles = in;
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
