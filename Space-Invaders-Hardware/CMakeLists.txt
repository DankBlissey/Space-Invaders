add_library(Space-Invaders-Hardware Hardware.h Hardware.cpp ShiftRegister.h ShiftRegister.cpp SpaceInvadersMemory.h SpaceInvadersMemory.cpp)
target_include_directories(Space-Invaders-Hardware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Space-Invaders-Hardware PRIVATE Intel8080)