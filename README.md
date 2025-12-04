# SpaceInvaders
### This is an emulator for the space invaders arcade cabinet, written in C++23
To build it, you will need CMake Version 3.20 or higher, as well as Ninja. 

![](Media/example.gif)

## Instructions for how to run:
vcpkg is already included as a git submodule so all you need to do is:  

- `git clone --recursive https://github.com/DankBlissey/Space-Invaders.git`  
- `cd Space-Invaders`  
- Add your ROM files to the SpaceInvadersROM folder.  
- `cmake --list-presets` which will show you which presets are available (a release and debug preset for your platform)  
- `cmake --preset <chosen preset>`  
- `cmake --build --preset <chosen preset>`  

If all things go well, you will find SpaceInvaders.exe in `Space-Invaders/out/build/<chosen preset>/Emulator/`  
Just run the .exe file to play!

#### If you have added the ROM files after having run `cmake --preset <chosen preset>` then you will need to re-run this command, otherwise the Emulator won't find them

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
Currently the emulator is running in black and white, in 3:4 aspect ratio as per the cabinet CRT screen, it does not currently have audio capabilities.  
### Things to add:  
- Visual effects: colour overlays, a background, crt scanlines, etc  
- Audio  

Enjoy!