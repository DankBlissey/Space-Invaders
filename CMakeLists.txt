# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SpaceInvaders")

# Include sub-projects.
add_subdirectory ("Intel8080")

set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    # warning level 4
    add_compile_options(/W4 /WX /external:W3 /W44365)
else()
    # additional warnings
    add_compile_options(-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion)
endif()
