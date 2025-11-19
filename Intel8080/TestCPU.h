#pragma once
#include "CPU.h"

class TestCPU: public CPU {
    public:
        friend struct Intel_8080_State;
        uint16_t getPc();
        uint16_t getSp();
        unsigned long getCycles();

        uint8_t getMem(uint16_t);

        uint16_t getPairB();
        uint16_t getPairD();
        uint16_t getPairH();
        uint16_t getPairPSW();

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

        void setMem(uint16_t, uint8_t);

        void setPairB(uint16_t);
        void setPairD(uint16_t);
        void setPairH(uint16_t);
        void setPairPSW(uint16_t);

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