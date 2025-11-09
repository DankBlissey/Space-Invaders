#include <catch2/catch_test_macros.hpp>
#include "Intel_8080_State.cpp"

TEST_CASE("CMC Complement Carry", "[opcodes, carry]") {
    CPU testCpu = CPU();
    testCpu.init();
    testCpu.writeMem(0x000, 0x3F);
    testCpu.writeMem(0x001, 0x3F);
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(0).stateEquals(testCpu));
    testCpu.cycle();
    REQUIRE(Intel_8080_State().with_Carry(1).with_pc(1).stateEquals(testCpu));
    testCpu.cycle();
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(2).stateEquals(testCpu));
}