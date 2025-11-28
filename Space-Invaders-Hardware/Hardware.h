#include "../CPU.h"
#include "SpaceInvadersMemory.h"
#include "ShiftRegister.h"

// Class representing all the hardware for space invaders
class Hardware {
    public:
        Hardware();
    private:
        SpaceInvadersMemory memory {SpaceInvadersMemory()};
        CPU intel8080 {CPU(memory)};
        ShiftRegister shiftRegister {ShiftRegister()};
};