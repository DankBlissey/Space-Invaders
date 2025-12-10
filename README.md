# Invaders From Outer Space
### This is an emulator for the Space Invaders arcade cabinet, written in C++   
It uses vcpkg for package management, SDL3 for rendering and audio, and Catch2 for automated testing.   
It features an accurate, cycle-counting, Intel 8080 CPU emulator  

![](Media/invadersGifShort.gif)  

## Beta Version 1.0.0 is here!  
You can find it in [Releases](https://github.com/DankBlissey/Invaders-From-Outer-Space/releases). It includes zip files for Windows and Linux binaries.  
## Install instructions  
### Disclaimer! The ROM and audio files are not included within the project as they are copyrighted material. You will have to source these files yourself and you do so at your own risk.  
- Extract the contents of the zip file
- Add your ROM files to the folder `SpaceInvadersROM` (file names should be: `9316b-0869_m739h.h1`, `9316b-0856_m739g.g1`, `9316b-0855_m739f.f1`, `9316b-0854_m739e.e1`, refer to the [ROM readme file](SpaceInvadersROM/README.txt) for more details)  
- Add your audio files `0.wav` - `9.wav` to the folder `Audio`. (Refer to the [Audio readme file](Audio/readme.txt) for more details)  
- Run the executable `InvadersFromOuterSpace.exe`  

If you would rather not use binaries from a (potentially) untrusted source, that is fully understandable, and you can build the program from the source code yourself:  
## Build From Source Instructions
To build the program, you will need CMake Version 3.26 or higher, as well as Ninja.  
vcpkg is already included as a git submodule, so it does not need to be installed on your machine beforehand.  

- `git clone --recursive https://github.com/DankBlissey/Space-Invaders.git`  
- `cd Space-Invaders`  
- Add your ROM files to the `SpaceInvadersROM` folder. See the [ROM readme](SpaceInvadersROM/README.txt) for details  
- Add your audio files to the `Audio` folder. See the [Audio readme](Audio/readme.txt) for details  
- `cmake --list-presets` which will show you which presets are available (a release and debug preset for your platform)  
- `cmake --preset <chosen preset>`  
- `cmake --build --preset <chosen preset>`

(Most IDEs will automate much of this process, so you will, most likely, just have to choose the debug or release preset and build it)  

If all things go well, you will find SpaceInvaders.exe in `Space-Invaders/out/build/<chosen preset>/Emulator`  
Just run the .exe file to play!  

#### If you have added the ROM and/or audio files after running `cmake --preset <chosen preset>` then you will need to re-run this command, otherwise the Emulator won't find them.   

#### Other buildable executables include `Intel8080TestROMs.exe` which is a program to run various CPU test ROMs to verify CPU accurate behaviour and `tests.exe` which are automated tests for the CPU opcodes written using the catch2 library.  

## Controls:
C = add credit  
1 = start in one-player mode  
2 = start in two-player mode  
a = left  
d = right  
space = fire  
t = tilt (simulates the cabinet being tilted, which results in a game over)  
esc = quit  

## State of project:
Currently, the emulator is running well, in a dynamically resizable window with a background and colour overlay, and with audio.  
Audio volume may be inconsistent.  
### Things to add:  
- More consistent audio volume  
- Visual effects: CRT scanlines, glow effect, etc  
- GUI for setting up ROM and Audio files (?)  
- Windows installer (?)  
- Controller support (?)  

Enjoy!
