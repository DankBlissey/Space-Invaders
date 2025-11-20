#include <catch2/catch_test_macros.hpp>
#include "Intel_8080_State.h"
#include <bit>

TestCPU testCpu = TestCPU();

TEST_CASE("CMC Complement Carry", "[opcodes, carry]") {
    testCpu.init();
    testCpu.setMem(0x000, 0x3F);
    testCpu.setMem(0x001, 0x3F);
    // Carry begins at 0
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(0).stateEquals(testCpu));
    testCpu.cycle();
    // CMC flips from 0 to 1
    REQUIRE(Intel_8080_State().with_Carry(1).with_pc(1).stateEquals(testCpu));
    testCpu.cycle();
    // CMC flips back from 1 to 0
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(2).stateEquals(testCpu));
}

TEST_CASE("STC Set Carry", "[opcodes, carry]") {
    testCpu.init();
    testCpu.setMem(0x000, 0x37);
    testCpu.setMem(0x001, 0x37);
    // Carry begins at 0
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(0).stateEquals(testCpu));
    testCpu.cycle();
    // STC sets carry to 1
    REQUIRE(Intel_8080_State().with_Carry(1).with_pc(1).stateEquals(testCpu));
    testCpu.cycle();
    // STC does nothing, carry is already at 1
    REQUIRE(Intel_8080_State().with_Carry(1).with_pc(2).stateEquals(testCpu));
}

// Zero, Sign, Parity, AuxCarry flags affected
// 0-3 4 and C
TEST_CASE("INR Increment Register or Memory (values)", "[opcodes, singleRegInstruction, values]") {
    testCpu.init();
    SECTION("INR on basic registers") {
        testCpu.setMem(0x000, 0x04); // INRB
        testCpu.setMem(0x001, 0x0C); // INRC
        testCpu.setMem(0x002, 0x14); // INRD
        testCpu.setMem(0x003, 0x1C); // INRE
        testCpu.setMem(0x004, 0x24); // INRH
        testCpu.setMem(0x005, 0x2C); // INRL
        testCpu.setMem(0x006, 0x3C); // INRA
        SECTION("INR correctly increments a register value") {
            testCpu.setAllReg(34);
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            REQUIRE(Intel_8080_State().with_B(35).with_C(35).with_D(35).with_E(35).with_H(35).with_L(35).with_A(35).stateEquals(testCpu));
        }
        SECTION("INR correctly increments negative numbers") {

            int8_t negative = -12;
            testCpu.setAllReg(std::bit_cast<uint8_t, int8_t>(negative));
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            int8_t inrNegative = -11;
            uint8_t inrConverted= std::bit_cast<uint8_t>(inrNegative);
            REQUIRE(Intel_8080_State().with_B(inrConverted).with_C(inrConverted).with_D(inrConverted).with_E(inrConverted).with_H(inrConverted).with_L(inrConverted).with_A(inrConverted).stateEquals(testCpu));
        }
        SECTION("INR correctly overflows") {
            int8_t maxNegative = -1;
            uint8_t notSigned = std::bit_cast<uint8_t>(maxNegative);
            testCpu.setAllReg(notSigned);
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            REQUIRE(Intel_8080_State().with_B(0).with_C(0).with_D(0).with_E(0).with_H(0).with_L(0).with_A(0).stateEquals(testCpu));
        }
    }
    SECTION("INR on memory location") {
        testCpu.setMem(0x00,0x34); // INRM
        testCpu.setH(0x3A);
        testCpu.setL(0x7C);
        SECTION("INR correctly increments a memory value") {
            testCpu.setMem(0x3A7C, 34);
            testCpu.cycle();
            REQUIRE(testCpu.getMem(0x3A7C) == 35);
        }
        SECTION("INR correctly increments a negative memory value") {
            int8_t negative = -12;
            testCpu.setMem(0x3A7C, std::bit_cast<uint8_t>(negative)); 
            testCpu.cycle();
            int8_t inrNegative = -11;
            REQUIRE(testCpu.getMem(0x3A7C) == std::bit_cast<uint8_t>(inrNegative));
        }
        SECTION("INR correctly overflows") {
            int8_t maxNegative = -1;
            uint8_t notSigned = std::bit_cast<uint8_t>(maxNegative);
            testCpu.setMem(0x3A7C, notSigned);
            testCpu.cycle();
            REQUIRE(testCpu.getMem(0x3A7C) == 0);
        }
    }
}

TEST_CASE("INR Increment Register or Memory (flags)", "[opcodes, singleRegInstruction, flags]") {
    testCpu.init();
    testCpu.setMem(0x000, 0x04); // INRB
    SECTION("Case 1: -1 -> 0") {
        testCpu.setB(255);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 0);
        REQUIRE(testCpu.getZero() == true);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getCarry() == false);
    }
    SECTION("Case 2: 79 -> 80") {
        testCpu.setB(79);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 80);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
    SECTION("Case 3: 78 -> 79") {
        testCpu.setB(78);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 79);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == false);
        REQUIRE(testCpu.getAuxCarry() == false);
    }
    SECTION("Case 4: 127 -> -128") {
        testCpu.setB(127);
        testCpu.cycle();
        int8_t minus = -128;
        REQUIRE(testCpu.getB() == std::bit_cast<uint8_t>(minus));
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == true);
        REQUIRE(testCpu.getParity() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getCarry() == false);
    }
}

TEST_CASE("DCR Decrement Register or Memory (values)", "[opcodes, singleRegInstructions, values]") {
    testCpu.init();
    SECTION("DCR on basic registers") {
        testCpu.setMem(0x000, 0x05); // DCRB
        testCpu.setMem(0x001, 0x0D); // DCRC
        testCpu.setMem(0x002, 0x15); // DCRD
        testCpu.setMem(0x003, 0x1D); // DCRE
        testCpu.setMem(0x004, 0x25); // DCRH
        testCpu.setMem(0x005, 0x2D); // DCRL
        testCpu.setMem(0x006, 0x3D); // DCRA
        SECTION("DCR correctly decrements on all registers") {
            testCpu.setAllReg(34);
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            REQUIRE(Intel_8080_State().with_B(33).with_C(33).with_D(33).with_E(33).with_H(33).with_L(33).with_A(33).stateEquals(testCpu));
        }
    }
    SECTION("DCR on memory") {
        testCpu.setMem(0x00,0x35); // DCRM
        testCpu.setH(0x3A);
        testCpu.setL(0x7C);
        SECTION("DCR correctly decrements a memory value") {
            testCpu.setMem(0x3A7C, 34);
            testCpu.cycle();
            REQUIRE(testCpu.getMem(0x3A7C) == 33);
        }
    }
}

TEST_CASE("DCR Decrement Register or Memory (flags)", "[opcodes, singleRegInstructions, flags]") {
    testCpu.init();
    testCpu.setMem(0x000, 0x05); // DCRB
    SECTION("Case 1: 80 -> 79") {
        testCpu.setB(80);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 79);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
    SECTION("Case 2: 1 -> 0") {
        testCpu.setB(1);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 0);
        REQUIRE(testCpu.getZero() == true);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == false);
    }
    SECTION("Case 3: 0 -> -1") {
        testCpu.setB(0);
        testCpu.cycle();
        int8_t minus = -1;
        REQUIRE(testCpu.getB() == std::bit_cast<uint8_t>(minus));
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == true);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
}

TEST_CASE("CMA Complement Accumulator", "[opcodes, singleRegInstructions]") {
    testCpu.init();
    testCpu.setMem(0x000, 0x2F); // CMA
    SECTION("Case 1:") {
        testCpu.setA(0b01001101);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b10110010);
    }
    SECTION("Case 2:") {
        testCpu.setA(0b10010111);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b01101000);
    }
    SECTION("Case 3:") {
        testCpu.setA(0b11111111);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b00000000);
    }
}

TEST_CASE("DAA Decimal Adjust Accumulator", "[opcodes, singleRegInstructions]") {
    testCpu.init();
    testCpu.setMem(0x000, 0x27);
    SECTION("Case: 1, Provided in 8080 manual") {
        testCpu.setA(0x9B);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 1);
        REQUIRE(testCpu.getCarry() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == false);
    }
    SECTION("Case: 2") {
        testCpu.setA(1);
        testCpu.setAuxCarry(true);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 7);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getAuxCarry() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == false);
    }
    SECTION("Case: 3") {
        testCpu.setA(0b00110101);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 53);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getAuxCarry() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == true);
    }
    SECTION("Case: 4") {
        testCpu.setA(0b01111101);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b10000011);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getSign() == true);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == false);
    }
}

TEST_CASE("MOV Move data from one register or memory location to another", "[opcodes, singleRegInstructions]") {
    testCpu.init();
    SECTION("Copy value across all registers") {
        uint8_t testValue = 0x3D;
        testCpu.setB(testValue);
        testCpu.setMem(0x000, 0x48); // MOV CB
        testCpu.setMem(0x001, 0x51); // MOV DC
        testCpu.setMem(0x002, 0x5A); // MOV ED
        testCpu.setMem(0x003, 0x63); // MOV HE
        testCpu.setMem(0x004, 0x6C); // MOV LH
        testCpu.setMem(0x005, 0x7D); // MOV AL
        for (int i = 0; i < 6; i++) {
            testCpu.cycle();
        }
        REQUIRE(Intel_8080_State().with_B(testValue).with_C(testValue).with_D(testValue).with_E(testValue).with_H(testValue).with_L(testValue).with_A(testValue).stateEquals(testCpu));
    }
    SECTION("Copy from register to mem") {
        testCpu.setB(0x3D);
        testCpu.setH(0x4F);
        testCpu.setL(0x01);
        testCpu.setMem(0x000, 0x70); // MOV MB
        testCpu.cycle();
        REQUIRE(testCpu.getMem(0x4F01) == 0x3D);
    }
    SECTION("Copy from mem to register") {
        testCpu.setE(0x3D);
        testCpu.setH(0x5A);
        testCpu.setL(0xFF);
        testCpu.setMem(0x5AFF, 0x20);
        testCpu.setMem(0x000, 0x5E); // MOV EM
        testCpu.cycle();
        REQUIRE(testCpu.getE() == 0x20);
    }
}

TEST_CASE("STAX Store accumulator to memory", "[opcodes, singleRegInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setB(0x3F);
        testCpu.setC(0x16);
        testCpu.setA(0xFF);
        testCpu.setMem(0x3F16, 0x20);
        testCpu.setMem(0x000, 0x02); // STAX B
        testCpu.cycle();
        REQUIRE(testCpu.getPairB() == 0x3F16);
        REQUIRE(testCpu.getMem(testCpu.getPairB()) == 0xFF);
    }
}

TEST_CASE("LDAX Load accumulator with data from memory", "[opcodes, singleRegInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setD(0x93);
        testCpu.setE(0x8B);
        testCpu.setA(0xFF);
        testCpu.setMem(0x938B, 0x20);
        testCpu.setMem(0x000, 0x1A); // LDAX D
        testCpu.cycle();
        REQUIRE(testCpu.getPairD() == 0x938B);
        REQUIRE(testCpu.getA() == 0x20);
    }
}

TEST_CASE("ADD Add register or memory to accumulator", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example 1") {
        testCpu.setD(0x2E);
        testCpu.setA(0x6C);
        testCpu.setMem(0x000, 0x82); // ADD D
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0x9A);
        REQUIRE(testCpu.getSign() == true);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
    SECTION("Manual example 2") {
        uint8_t value {9};
        testCpu.setA(value);
        testCpu.setMem(0x000, 0x87); // ADD A
        testCpu.cycle();
        REQUIRE(testCpu.getA() == value * 2);
    }
}

TEST_CASE("ADC Add register or memory to accumulator with carry", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setC(0x3D);
        testCpu.setA(0x42);
        testCpu.setMem(0x000, 0x89); // ADC C
        SECTION("Without carry") {
            testCpu.setCarry(false);
            testCpu.cycle();
            REQUIRE(testCpu.getA() == 0x7F);
            REQUIRE(testCpu.getCarry() == false);
            REQUIRE(testCpu.getSign() == false);
            REQUIRE(testCpu.getZero() == false);
            REQUIRE(testCpu.getParity() == false);
            REQUIRE(testCpu.getAuxCarry() == false);
        }
        SECTION("With carry") {
            testCpu.setCarry(true);
            testCpu.cycle();
            REQUIRE(testCpu.getA() == 0x80);
            REQUIRE(testCpu.getCarry() == false);
            REQUIRE(testCpu.getSign() == true);
            REQUIRE(testCpu.getZero() == false);
            REQUIRE(testCpu.getParity() == false);
            REQUIRE(testCpu.getAuxCarry() == true);
        }
    }
}

TEST_CASE("SUB Subtract register or memory from accumulator", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setA(0x3E);
        testCpu.setMem(0x000, 0x97); // SUB A
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == true);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
}

TEST_CASE("SBB Subtract register or memory from accumulator with borrow", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manaul example") {
        testCpu.setL(0x02);
        testCpu.setA(0x04);
        testCpu.setCarry(true);
        testCpu.setMem(0x000, 0x9D); // SBB L
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 1);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
}

TEST_CASE("ANA Logical and register or memory with accumulator", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setA(0xFC);
        testCpu.setC(0x0F);
        testCpu.setMem(0x000, 0xA1); // ANA C
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0x0C);
    }
}

TEST_CASE("XRA Logical exclusive-or register or memory with accumulator", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example 1") {
        testCpu.setA(0x04);
        testCpu.setB(0xFF);
        testCpu.setC(0xA5);
        testCpu.setMem(0x000, 0xAF); // XRA A
        testCpu.setMem(0x001, 0x47); // MOV B A
        testCpu.setMem(0x002, 0x4F); // MOV C A
        for (int i = 0; i < 3; i++) {
            testCpu.cycle();
        }
        REQUIRE(Intel_8080_State().with_A(0).with_B(0).with_C(0).stateEquals(testCpu));
    }
    SECTION("Manual example 2") {
        uint8_t value {0xA4};
        uint8_t flippedValue = ~value;
        testCpu.setA(0xFF);
        testCpu.setB(value);
        testCpu.setMem(0x000, 0xA8); // XRA B
        REQUIRE(testCpu.getA() == 255);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == flippedValue);
    }
}

TEST_CASE("ORA Logical or register or memory with accumulator", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setC(0x0F);
        testCpu.setA(0x33);
        testCpu.setMem(0x000, 0xB1); // ORA C
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0x3F);
    }
}

TEST_CASE("CMP Compare register or memory with accumulator", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setE(0x05);
        testCpu.setMem(0x000, 0xBB); // CMP E
        SECTION("Example 1") {
            testCpu.setA(0x0A);
            testCpu.cycle();
            REQUIRE(testCpu.getA() == 0x0A);
            REQUIRE(testCpu.getE() == 0x05);
            REQUIRE(testCpu.getCarry() == false);
            REQUIRE(testCpu.getZero() == false);
        }
        SECTION("Example 2") {
            testCpu.setA(0x02);
            testCpu.cycle();
            REQUIRE(testCpu.getA() == 0x02);
            REQUIRE(testCpu.getE() == 0x05);
            REQUIRE(testCpu.getCarry() == true);
            REQUIRE(testCpu.getZero() == false);
        }
        SECTION("Example 3") {
            uint8_t value = 0x1B;
            uint8_t negative = ~value + 1;
            testCpu.setA(negative);
            testCpu.cycle();
            REQUIRE(testCpu.getA() == negative);
            REQUIRE(testCpu.getE() == 0x05);
            REQUIRE(testCpu.getCarry() == false);
            REQUIRE(testCpu.getZero() == false);
        }
    }
}

TEST_CASE("RLC Rotate accumulator left", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setA(0xF2);
        testCpu.setMem(0x000, 0x07); // RLC
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0xE5);
        REQUIRE(testCpu.getCarry() == true);
    }
}

TEST_CASE("RRC Rotate accumulator right", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setA(0xF2);
        testCpu.setMem(0x000, 0x0F); // RRC
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0x79);
        REQUIRE(testCpu.getCarry() == false);
    }
}

TEST_CASE("RAL Rotate accumulator left through carry", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setA(0xB5);
        testCpu.setCarry(false);
        testCpu.setMem(0x000, 0x17); // RAL
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0x6A);
        REQUIRE(testCpu.getCarry() == true);
    }
}

TEST_CASE("RAR Rotate accumulator right through carry", "[opcodes, regOrMemToAccumulatorInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setA(0x6A);
        testCpu.setCarry(true);
        testCpu.setMem(0x000, 0x1F);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0xB5);
        REQUIRE(testCpu.getCarry() == false);
    }
}

TEST_CASE("PUSH Push data onto stack", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        SECTION("Example 1") {
            testCpu.setD(0x8F);
            testCpu.setE(0x9D);
            testCpu.setSp(0x3A2C);
            testCpu.setMem(0x000, 0xD5); // PUSH D
            testCpu.cycle();
            REQUIRE(testCpu.getSp() == 0x3A2A);
            REQUIRE(testCpu.getMem(0x3A2B) == 0x8F);
            REQUIRE(testCpu.getMem(0x3A2A) == 0x9D);
        }
        SECTION("Example 2") {
            testCpu.setA(0x1F);
            testCpu.setSp(0x502A);
            testCpu.setCarry(true);
            testCpu.setZero(true);
            testCpu.setParity(true);
            testCpu.setSign(false);
            testCpu.setAuxCarry(false);
            testCpu.setMem(0x000, 0xF5); // PUSH PSW
            testCpu.cycle();
            REQUIRE(testCpu.getSp() == 0x5028);
            REQUIRE(testCpu.getMem(0x5029) == 0x1F);
            REQUIRE(testCpu.getMem(0x5028) == 0x47);
        }
    }
}

TEST_CASE("POP Pop data off stack", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        SECTION("Example 1") {
            testCpu.setMem(0x1239, 0x3D);
            testCpu.setMem(0x123A, 0x93);
            testCpu.setSp(0x1239);
            testCpu.setMem(0x000, 0xE1); // POP H
            testCpu.cycle();
            REQUIRE(testCpu.getSp() == 0x123B);
            REQUIRE(testCpu.getL() == 0x3D);
            REQUIRE(testCpu.getH() == 0x93);
        }
        SECTION("Example 2") {
            testCpu.setMem(0x2C00, 0xC3);
            testCpu.setMem(0x2C01, 0xFF);
            testCpu.setSp(0x2C00);
            testCpu.setMem(0x000, 0xF1); // POP PSW
            testCpu.cycle();
            REQUIRE(testCpu.getA() == 0xFF);
            REQUIRE(testCpu.getSign() == true);
            REQUIRE(testCpu.getZero() == true);
            REQUIRE(testCpu.getAuxCarry() == false);
            REQUIRE(testCpu.getParity() == false);
            REQUIRE(testCpu.getCarry() == true);
        }
    }
}

TEST_CASE("DAD Double add", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setH(0xA1);
        testCpu.setL(0x7B);
        SECTION("Example 1") {
            testCpu.setB(0x33);
            testCpu.setC(0x9F);
            testCpu.setMem(0x000, 0x09); // DAD B
            testCpu.cycle();
            REQUIRE(testCpu.getH() == 0xD5);
            REQUIRE(testCpu.getL() == 0x1A);
            REQUIRE(testCpu.getCarry() == false);
        }
        SECTION("Example 2") {
            uint16_t value = testCpu.getPairH();
            value = value << 1;
            testCpu.setMem(0x000, 0x29); // DAD H
            testCpu.cycle();
            REQUIRE(testCpu.getPairH() == value);
        }
    }
}

TEST_CASE("INX Increment register pair", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        SECTION("Example 1") {
            testCpu.setD(0x38);
            testCpu.setE(0xFF);
            testCpu.setMem(0x000, 0x13); // INX D
            testCpu.cycle();
            REQUIRE(testCpu.getD() == 0x39);
            REQUIRE(testCpu.getE() == 0x00);
        }
        SECTION("Example 2") {
            testCpu.setSp(0xFFFF);
            testCpu.setMem(0x000, 0x33); // INX SP
            testCpu.cycle();
            REQUIRE(testCpu.getSp() == 0x0000);
        }
    }
    SECTION("Example 3") {
        testCpu.setB(0x38);
        testCpu.setC(0xA4);
        testCpu.setMem(0x000, 0x03); // INX B
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 0x38);
        REQUIRE(testCpu.getC() == 0xA5);
    }
}

TEST_CASE("DCX Decrement register pair", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setH(0x98);
        testCpu.setL(0x00);
        testCpu.setMem(0x000, 0x2B); // DCX H
        testCpu.cycle();
        REQUIRE(testCpu.getH() == 0x97);
        REQUIRE(testCpu.getL() == 0xFF);
    }
}

TEST_CASE("XCHG Exchange registers", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setD(0x33);
        testCpu.setE(0x55);
        testCpu.setH(0x00);
        testCpu.setL(0xFF);
        testCpu.setMem(0x000, 0xEB); // XCHG
        REQUIRE(testCpu.getPairD() == 0x3355);
        REQUIRE(testCpu.getPairH() == 0x00FF);
        testCpu.cycle();
        REQUIRE(testCpu.getPairD() == 0x00FF);
        REQUIRE(testCpu.getPairH() == 0x3355);
    }
}

TEST_CASE("XTHL Exchange stack", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setSp(0x10AD);
        testCpu.setH(0x0B);
        testCpu.setL(0x3C);
        testCpu.setMem(0x10AD, 0xF0);
        testCpu.setMem(0x10AE, 0x0D);
        testCpu.setMem(0x000, 0xE3); // XTHL
        testCpu.cycle();
        REQUIRE(testCpu.getMem(0x10AD) == 0x3C);
        REQUIRE(testCpu.getMem(0x10AE) == 0x0B);
        REQUIRE(testCpu.getH() == 0x0D);
        REQUIRE(testCpu.getL() == 0xF0);
    }
}

TEST_CASE("SPHL Load sp from H and L", "[opcodes, regPairInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setH(0x50);
        testCpu.setL(0x6C);
        testCpu.setMem(0x000, 0xF9); // SPHL
        testCpu.cycle();
        REQUIRE(testCpu.getPairH() == 0x506C);
        REQUIRE(testCpu.getSp() == 0x506C);
    }
}

TEST_CASE("LXI Load 16 bit value into register pair", "[opcodes, immediateInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        SECTION("Example 1") {
            testCpu.setMem(0x000, 0x21); // LXI H
            testCpu.setMem(0x001, 0x03);
            testCpu.setMem(0x002, 0x01);
            testCpu.cycle();
            REQUIRE(testCpu.getPairH() == 0x0103);
            REQUIRE(testCpu.getPc() == 0x003);
        }
        SECTION("Example 2") {
            testCpu.setMem(0x000, 0x31); // LXI SP
            testCpu.setMem(0x001, 0xBC);
            testCpu.setMem(0x002, 0x3A);
            testCpu.cycle();
            REQUIRE(testCpu.getSp() == 0x3ABC);
            REQUIRE(testCpu.getPc() == 0x003);
        }
    }
}

TEST_CASE("MVI Move immediate data", "[opcodes, immediateInstructions]") {
    testCpu.init();
    SECTION("Manual example") {
        testCpu.setMem(0x000, 0x26); // MVI H
        testCpu.setMem(0x001, 0x3C);
        testCpu.setMem(0x002, 0x2E); // MVI L
        testCpu.setMem(0x003, 0xF4);
        testCpu.setMem(0x004, 0x36); // MVI M
        testCpu.setMem(0x005, 0xFF);
        for (int i = 0; i < 3; i++) {
            testCpu.cycle();
        }
        REQUIRE(testCpu.getMem(0x3CF4) == 0xFF);
    }
}