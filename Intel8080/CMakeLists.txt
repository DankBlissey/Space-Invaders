# CMakeList.txt : CMake project for Intel8080, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Intel8080 "Intel8080.cpp" "Intel8080.h" "CPU.h" "CPU.cpp" "CPU_Opcodes.cpp" "Opcode_Cycles.cpp" "Bit_Calcs.cpp" "Intel_8080_State.cpp" )

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG b3fb4b9feafcd8d91c5cb510a4775143fdbef02f
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests Intel8080Tests.cpp "CPU.h" "CPU.cpp" "CPU_Opcodes.cpp" "Opcode_Cycles.cpp" "Bit_Calcs.cpp" "Intel_8080_State.cpp" )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

set_property(TARGET Intel8080 PROPERTY CXX_STANDARD 23)
set_property(TARGET tests PROPERTY CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

catch_discover_tests(tests)

# TODO: Add tests and install targets if needed.