add_library(Space-Invaders-Hardware Hardware.h Hardware.cpp ShiftRegister.h ShiftRegister.cpp SpaceInvadersMemory.h SpaceInvadersMemory.cpp SoundChip.h)
target_include_directories(Space-Invaders-Hardware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TARGET Space-Invaders-Hardware PROPERTY CXX_STANDARD 23)

target_link_libraries(Space-Invaders-Hardware PRIVATE Intel8080)