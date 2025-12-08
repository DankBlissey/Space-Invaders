find_package(SDL3 CONFIG REQUIRED)

find_package(SDL3_image CONFIG REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/SpaceInvadersROM
     DESTINATION ${CMAKE_BINARY_DIR}/Emulator)

file(COPY ${CMAKE_SOURCE_DIR}/Media/invadersBitMapDark.bmp
     DESTINATION ${CMAKE_BINARY_DIR}/Emulator)

add_executable(SpaceInvaders SpaceInvaders.cpp LTimer.h LTimer.cpp)

set_property(TARGET SpaceInvaders PROPERTY CXX_STANDARD 23)

target_link_libraries(SpaceInvaders PRIVATE SDL3::SDL3 SDL3_image::SDL3_image-static Space-Invaders-Hardware)