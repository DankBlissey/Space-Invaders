# SpaceInvaders
### This is an emulator for the space invaders arcade cabinet, written in C++23
To build it, you will need CMake Version 3.20 or higher, as well as Ninja. 

## Instructions for how to run:
vcpkg is already included as a git submodule so all you need to do is:  

-Add your rom files to the SpaceInvadersROM folder  
-Use CMake to configure and build  
-Run the compiled SpaceInvaders.exe file  

## Controls:
C = add credit  
1 = start in one player mode  
2 = start in two player mode  
a = left  
d = right  
space = fire  
t = tilt (simulates the cabinet being tilted which results in a game over)  
esc = quit  

## State of project:
Currently the emulator is fully running in black and white, in 3:4 aspect ratio as per the cabinet CRT screen, current development now revolves around adding visual effects such as colour overlays, a background, crt scanlines, etc. 

Enjoy!