find_package(SDL3 CONFIG REQUIRED)

add_executable(SpaceInvaders "SpaceInvaders.cpp" "Hardware.h" "Hardware.cpp" "ShiftRegister.h" "ShiftRegister.cpp" "SpaceInvadersMemory.h" "SpaceInvadersMemory.cpp")

target_link_libraries(SpaceInvaders PRIVATE SDL3::SDL3 Intel8080)