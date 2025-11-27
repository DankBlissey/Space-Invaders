# CMakeList.txt : CMake project for Intel8080, include source and define
# project specific logic here.
#

# Copy the ROMs folder to the binary directory
file(COPY ${CMAKE_SOURCE_DIR}/Intel8080/CPU-Test-ROMs
     DESTINATION ${CMAKE_BINARY_DIR}/Intel8080)

# Add source to this project's executable.
add_executable (Intel8080TestROMs "Intel8080TestROMs.cpp" "CPU.h" "CPU.cpp" "CPUOpcodes.cpp" "OpcodeTableValues.cpp" "Memory.h" "Memory.cpp")

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG b3fb4b9feafcd8d91c5cb510a4775143fdbef02f
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests "Intel8080Tests.cpp" "CPU.h" "CPU.cpp" "CPUOpcodes.cpp" "OpcodeTableValues.cpp" "TestCPU.h" "TestCPU.cpp")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

#add_executable(compare "CompareEmulators.cpp" "ExampleEmulator/i8080.h" "ExampleEmulator/i8080.c" "CPU.h" "CPU.cpp" "CPUOpcodes.cpp" "OpcodeTableValues.cpp" "TestCPU.h" "TestCPU.cpp")

set_property(TARGET Intel8080TestROMs PROPERTY CXX_STANDARD 23)
set_property(TARGET tests PROPERTY CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

catch_discover_tests(tests)

# TODO: Add tests and install targets if needed.