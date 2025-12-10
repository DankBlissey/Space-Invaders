# Invaders From Outer Space
### This is an emulator for the Space Invaders arcade cabinet, written in C++  
To build it, you will need CMake Version 3.26 or higher, as well as Ninja.  
It uses vcpkg for package management, SDL3 for rendering and audio, and Catch2 for automated testing.   
It features an accurate, cycle-counting, Intel 8080 CPU emulator  

![](Media/invadersGifNew.gif)  

## Install  
As the project is still ongoing, I have not released any binaries, so for now, you will have to settle with building it from source.  
Once I am satisfied with the state of the emulator, I will make a full release with binaries for different platforms. 

## Build Instructions
vcpkg is already included as a git submodule so all you need to do is:  

- `git clone --recursive https://github.com/DankBlissey/Space-Invaders.git`  
- `cd Space-Invaders`  
- Add your ROM files to the `SpaceInvadersROM` folder. Refer to the `readme.txt` file in the `SpaceInvadersROM` folder for the file naming format  
- Add your audio files to the `Audio` folder. The files should be 0.wav-9.wav  
- `cmake --list-presets` which will show you which presets are available (a release and debug preset for your platform)  
- `cmake --preset <chosen preset>`  
- `cmake --build --preset <chosen preset>`

(Most IDEs will automate much of this process, so you will, most likely, just have to choose the debug or release preset and build it)  

If all things go well, you will find SpaceInvaders.exe in `Space-Invaders/out/build/<chosen preset>/Emulator`  
Just run the .exe file to play!  

#### If you have added the ROM and/or audio files after running `cmake --preset <chosen preset>` then you will need to re-run this command, otherwise the Emulator won't find them.   

### I cannot include the ROM or audio files within the project as they are copyrighted material. You will have to source these yourself and do so at your own risk.  

### Other buildable executables include `Intel8080TextROMs.exe` which is a program to run various CPU test ROMs to verify CPU accurate behaviour and `tests.exe` which are automated tests for the CPU opcodes written using the catch2 library.  

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
Audio volume is currently quite inconsistent; some sounds are far too loud.  
### Things to add:  
- More consistent audio volume  
- Visual effects: CRT scanlines, glow effect, etc  (?)
- GUI for setting up ROM and Audio files (?)  
- Windows installer (?)  
- Controller support (?)  

Enjoy!
