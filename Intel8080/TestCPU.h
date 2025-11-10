#pragma once
#include "CPU.h"
#include <cstdint>

class TestCPU: public CPU {
    public:
        friend struct Intel_8080_State;
        uint16_t getPc();
        uint16_t getSp();
        unsigned long getCycles();

        uint8_t getB();
        uint8_t getC();
        uint8_t getD();
        uint8_t getE();
        uint8_t getH();
        uint8_t getL();
        uint8_t getA();

        bool getSign();
        bool getZero();
        bool getAuxCarry();
        bool getParity();
        bool getCarry();

        void setPc(uint16_t);
        void setSp(uint16_t);
        void setCycles(unsigned long);

        void setAllReg(uint8_t);

        void setB(uint8_t);
        void setC(uint8_t);
        void setD(uint8_t);
        void setE(uint8_t);
        void setH(uint8_t);
        void setL(uint8_t);
        void setA(uint8_t);

        void setSign(bool);
        void setZero(bool);
        void setAuxCarry(bool);
        void setParity(bool);
        void setCarry(bool);
};